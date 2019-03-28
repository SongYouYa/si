#include "stdafx.h"
#include "PFilesystemView.h"
#include "logic/PDeviceLoader.h"

#include "./filter/PFsFilterBar.h"

#include "uicommon/filesystem/PFsItemWidget.h"
#include "uicommon/table/PTableWithStatusView.h"
#include "ui/content/filesytem/export/PExportOptionDialog.h"
#include "ui/content/filesytem/export/PExportFileSizeCalculator.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"
#include "ui/common/PFilePathChecker.h"

#include "TemplateFile/PTemplateFile.h"
#include "FileAnalyzer/PEncryptFile.h"
#include "filesystemshell/PFileProperty.h"
#include "filesystemshell/FileSystemShell.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/PDeviceTaskParams.h"

#include "base/PRecordTypeFactory.h"

#include "handler/PFsHandler.h"

using namespace devtask;
using namespace filesystem;

extern CPTabManager* g_mainTab;
extern CPRecord* g_exportFileFolder;

CPFilesystemView::CPFilesystemView(QWidget* parent)
	: QWidget(parent),
	_exportOptionDialog(nullptr)
{
	setObjectName("filesystem");

	CPTreeItemWidgetBase::setItemContentMargins(QMargins(5, 5, 5, 5));
#ifdef OS_PAPPLE
    //mac下间距需要设置大一些，否则界面会错位
    CPFsItemWidget::setItemSpacing(10);
#else
    CPFsItemWidget::setItemSpacing(5);
#endif
	CPFsItemWidget::setItemIndent(12);

	CPTreeWidget::setTreeMinWidth(200);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);

	CPFsFilterBar* filterBar = new CPFsFilterBar();
	_treelistWidget = new CPFsTreelistWidget(this, filterBar);
	_lineLoading = new CPLineLoadingWidget(filterBar);
	_treelistWidget->setHandlingWidget(_lineLoading);

	CPOperationButtonContainer* bottomContainer = new CPOperationButtonContainer();
	QWidget* buttonContainer = new QWidget();
	QHBoxLayout* buttonLayout = new QHBoxLayout();
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(10);
	_exportFileBox = new QCheckBox(tr("export file"));
	_exportFileBox->setChecked(true);
	_exportListBox = new QCheckBox(tr("export list"));
	_exportListBox->setChecked(true);
	_createZipBox = new QCheckBox(tr("create zip"));
	_startButton = CPOperationButton::newForensicButton();
	_startButton->setText(tr("export"));
	buttonLayout->addWidget(_exportFileBox);
	buttonLayout->addWidget(_exportListBox);
	buttonLayout->addWidget(_createZipBox);
	buttonLayout->addSpacing(5);
	buttonLayout->addWidget(_startButton);
	buttonContainer->setLayout(buttonLayout);
	            
	bottomContainer->addRightWidget(buttonContainer);
	_treelistWidget->addTableBottomWidget(bottomContainer);

	layout->addWidget(_treelistWidget);
	setLayout(layout);

#if 1
	//初始化时，就显示为loading
	_treelistWidget->setLoading(true);
#else
	connect(CPDeviceLoader::get(), &CPDeviceLoader::signalLoadStart, this, [this](){
		_treelistWidget->setLoading(true);
	}, Qt::QueuedConnection);
#endif
	connect(CPDeviceLoader::get(), SIGNAL(signalLoadFinished(bool)),
		this, SLOT(slotLoadFinished(bool)), Qt::QueuedConnection);


	//设置搜索应用文件的回调
	filesystem::CPFileProperty::get()->setParseAppCallback([](std::set<PSIHANDLE>& handles){
		tpfile::CPTemplateFile tpFile;
		tpFile.searchAllHandles(handles);
	});
	filesystem::CPFileProperty::get()->setParseEncryptCallback([](std::set<PSIHANDLE>& handles){
		pfile::CPEncryptFile::getAllEncryptHandles(handles);
	});

	//添加文件导出按钮信号连接
	connect(_startButton, SIGNAL(clicked()), this, SLOT(slotExportClicked()));
	connect(_createZipBox, SIGNAL(clicked()), this, SLOT(slotChangeZipBoxState()));
	connect(_exportFileBox, SIGNAL(clicked()), this, SLOT(slotChangeFileBoxState()));

	//当底层任务处理好所选文件之后，界面更新选中情况
	connect(this, &CPFilesystemView::signalStateChanged, this, [this](int stage) {
		if (stage == FE_STATE_HANDLE_PARSED) {
			_treelistWidget->updateCheckState();
		}
	}, Qt::QueuedConnection);

	filterBar->setPreCallback([this](){
		_treelistWidget->treeWidget()->setDisabled(true);
		_treelistWidget->tableWidget()->setLoading(true);
		_lineLoading->setLoading(true);
	});
}

void CPFilesystemView::showEvent(QShowEvent* e)
{
	QWidget::showEvent(e);
	qApp->processEvents();
	//展示文件系统界面时，再加载文件系统（在有些机器上，加载文件系统会崩溃，如果
	//不需要文件系统，不切换到文件系统页面，可以不加载文件系统
	CPFsHandler::get()->loadFs();
	qApp->processEvents();
}

void CPFilesystemView::slotChangeZipBoxState()
{
	//如果选择导出压缩文件，自动选择导出file选项
	if (_createZipBox->isChecked()){
		_exportFileBox->setChecked(true);
	}	
}

void CPFilesystemView::slotChangeFileBoxState()
{
	//如果不选择导出文件，导出压缩包也不选择
	if (!_exportFileBox->isChecked()){
		_createZipBox->setChecked(false);
	}
}

void CPFilesystemView::slotLoadFinished(bool suc)
{
	_treelistWidget->setLoading(false);
	if (suc) {
		std::vector<PSIHANDLE> handles;
		CPDeviceLoader::get()->getLoadedDeviceHandles(handles);
		_treelistWidget->addTreeRootItems(handles);
	}
}

void CPFilesystemView::slotExportClicked()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}

	//在这里进行任务的文件导出
	FileExportParam exportParam;
	//传递checkbox的状态
	exportParam.exportFile = _exportFileBox->isChecked();
	exportParam.createZip = _createZipBox->isChecked();
	std::vector<PSIHANDLE> rootHandles;
	//应该是获取所有的树节点handler
	CPDeviceLoader::get()->getLoadedDeviceHandles(rootHandles);
	for (auto it = rootHandles.begin(); it != rootHandles.end(); ++it) {
		ITEM_CHECK_STATE state = CFileSystem::ItemCheckState(*it);
		//发现选择状态的条目
		if (state != ITEM_CHECK_NONE) {
			exportParam.rootHandles.push_back(*it);
		}
	}
	if (exportParam.rootHandles.empty()) {
		CPMessageBox::info(tr("Please select export files."));
		return;
	}
	//获取导出路径
	
	if (!_exportOptionDialog) {
		_exportOptionDialog = new CPExportOptionDialog();
	}
	int ret = _exportOptionDialog->exec();
	if (ret != QDialog::Accepted) {
		return;
	}
	_exportOptionDialog->getExportParam(exportParam);
	if (exportParam.exportDir.empty()){
		CPMessageBox::info(tr("export file path can not empty!"));
		return;
	}
	//判断文件路径是否符合要求
	CPExportFileSizeCalculator helper;
	helper.update(exportParam.rootHandles);
	uint64 totalSize =  helper.getTotalSize();
	uint64 singleSize = helper.getSingleSize();
	bool isCheckedPassed = CPFilePathChecker::checkTargetPath(exportParam.exportDir,
		totalSize, singleSize);
	if (!isCheckedPassed){
		return;
	}
	if (!CPPath::createDirs(exportParam.exportDir)) {
		CPMessageBox::error(tr("export dir error!"));
		return;
	}

	exportParam.folderRecord = g_exportFileFolder;

	//创建任务
	TaskParamPtr fileExportTaskParam = std::make_shared<boost::any>(exportParam);
	CPTask *task = CPTaskController::get()->createTask(L"task.fileexport", fileExportTaskParam);
	
	//开始任务
	task->addEndCallback([](CPTask* task){
		g_exportFileFolder->removeFlag(RFlagNoShow);
	});
	_startButton->setDisabled(true);
	task->addUiEndCallback([this](CPTask* task){
		_startButton->setDisabled(false);
	});

	task->statusCtrl()->connectStageChanged([this](int stage){
		emit signalStateChanged(stage);
	});

	CPTaskController::get()->doTask(task);
#if 0
	g_mainTab->navToTab(PTAB_TASK);//切换页面
#endif
}
