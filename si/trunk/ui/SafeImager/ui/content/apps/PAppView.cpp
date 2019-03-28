#include "stdafx.h"
#include "PAppView.h"

#include "uicommon/record/PRecordItemWidget.h"
#include "uicommon/tree/PTreeWidget.h"
#include "uicommon/record/PAppTreeWidget.h"
#include "ui/common/filter/PFilterBarWidget.h"

#include "logic/PDeviceLoader.h"
#include "handler/PFsHandler.h"
#include "logic/case/PCaseCatagoryHandler.h"

#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"

#include "application/CommonAnalyzer/PAppParam.h"
#include "application/PTaskError.h"
#include "base/PRecordTypeFactory.h"
#include "siutility/PRecordHandler.h"


extern CPTabManager* g_mainTab;
extern CPRecord* g_appFolder; //加载文件系统后，获得的根节点

CPAppView::CPAppView(QWidget* parent)
	: QWidget(parent),
	_appRawRootRecord(nullptr),
	_appInited(false)
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);

	CPTreeWidget::setTreeMinWidth(200);
    CPRecordItemWidget::setItemIndent(16);
#ifdef OS_PAPPLE
    //mac下间距需要设置大一些，否则界面会错位
    CPRecordItemWidget::setItemSpacing(10);
#else
    CPRecordItemWidget::setItemSpacing(5);
#endif
	CPRecordItemWidget::setIconVisible(false);

	CPFilterBarWidget* filterBar = new CPFilterBarWidget();
	_appTreelist = new CPAppTreelistWidget(this, filterBar);
	_appTreelist->setObjectName("applist");

	CPOperationButtonContainer* bottomContainer = new CPOperationButtonContainer();

	_loadingWidget = new CPLoadingWidget();
	_loadingWidget->setVisible(false);
	bottomContainer->addLeftWidget(_loadingWidget);

	_startButton = CPOperationButton::newForensicButton();
	_startButton->setText(tr("pickup"));
	_startButton->setDisabled(true);
	bottomContainer->addRightWidget(_startButton);

	_appTreelist->addTableBottomWidget(bottomContainer);

	layout->addWidget(_appTreelist);
	setLayout(layout);

	_appTreelist->setLoading(true);

	connect(_startButton, SIGNAL(clicked()), this, SLOT(startAppTask()));

	connect(CPCaseCatagoryHandler::get(), SIGNAL(signalCatagoryChanged()),
		this, SLOT(slotCatagoryChanged()));
}

void CPAppView::initView()
{
	startInitTask();
}

void CPAppView::slotCatagoryChanged()
{
	std::set<std::wstring> cataApps;
	CPCaseCatagoryHandler::get()->getCatagoryApps(cataApps);
	if (cataApps.empty()) {
		return;
	}
	CPRecord* rootRecord = _appInited ? g_appFolder : _appRawRootRecord;
	siutil::CPRecordHandler::checkRecordByTypes(rootRecord, cataApps);
	_appTreelist->treeWidget()->updateItemCheckState();
}

void CPAppView::startInitTask()
{
	CPTask* task = CPTaskController::get()->createTask(L"task.initapptree", nullptr, false);
	_appRawRootRecord = task->rootRecord();

	task->addUiEndCallback([this](CPTask* task){
		_appTreelist->setLoading(false);
		CPRecord* rootRecord = task->rootRecord();
		for (CPRecord* child : rootRecord->getChildren()) {
			_appTreelist->addTreeRootItem(child);
		}
		_startButton->setDisabled(false);
	});
	CPTaskController::get()->doTask(task);
}

void CPAppView::startAppTask()
{
	if (!CPCaseHandler::checkCaseCreated()) {
		return;
	}
	assert(_appRawRootRecord);

	CPRecord* curRootRecord = _appInited ? g_appFolder : _appRawRootRecord;
	if (RECORD_CHECK_NONE == curRootRecord->getCheckState()) {
		CPMessageBox::warning(tr("Please select items to analyzer!"));
		return;
	}

	app::AppParam appParam;
	appParam.rawRootRecord = _appRawRootRecord;
	appParam.rootRecord = g_appFolder;
	appParam.appInited = _appInited;
	//创建任务
	TaskParamPtr taskParam = std::make_shared<boost::any>(appParam);
	CPTask* task = CPTaskController::get()->createTask(L"task.apps", taskParam);

	_loadingWidget->setVisible(true);
	_loadingWidget->start();
	_appTreelist->setRunning(true);
	_startButton->setDisabled(true);
	task->addUiEndCallback([this](CPTask* task){
		//重新初始化树节点
		if (!_appInited) {
			_appInited = true;
			g_appFolder->removeFlag(RFlagNoShow);
			//还没有根据文件系统初始化系统情况
			_appTreelist->treeWidget()->clear();
			for (CPRecord* child : g_appFolder->getChildren()) {
				_appTreelist->addTreeRootItem(child);
			}
		}
		else {
			_appTreelist->treeWidget()->updateItemsName();
		}
		
		_loadingWidget->stop();
		_loadingWidget->setVisible(false);
		_appTreelist->setRunning(false);
		_startButton->setDisabled(false);

		CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
		int errorCode = statusCtrl->errorCode();
		if (errorCode == TASK_ERROR_TOOL_LOST) {
			CPMessageBox::error(tr("Some password tools are lost, please close the anti-virus softwares and retry."));
		}
	});

	//开始数据分析时，再加载文件系统（在有些机器上，加载文件系统会崩溃，所以需要时，再加载文件系统）
	CPFsHandler::get()->loadFs();
	CPTaskController::get()->doTask(task);

#if 0
	g_mainTab->navToTab(PTAB_TASK);
#endif
}
