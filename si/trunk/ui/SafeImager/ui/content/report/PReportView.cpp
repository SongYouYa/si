#include "stdafx.h"
#include "PReportView.h"

#include "uicommon/factory/PPageFactory.h"
#include "uicommon/record/PRecordItemWidget.h"
#include "uicommon/widget/PCopyContextMenu.h"
#include "logic/PReportStatus.h"
#include "siutility/PRecordHandler.h"
#include "siutility/PDevicePath.h"
#include "ui/common/PFilePathChecker.h"

#include "./PReportRecordHandler.h"
#include "./PReportCreater2.h"
#include "./PExportReportDialog.h"


extern std::string IdPre = "_ID_";
const QString ContentHtml = "content.html";
extern const std::wstring ReportDirName;

CPReportView::CPReportView(QWidget* parent)
	: QWidget(parent),
	_startType(StartByNone)
{
	setObjectName("report_view");

	_treeWidget = new CPAppTreeWidget();

	_browser = new QTextBrowser();
	bool isr = _browser->isReadOnly();
	_browser->setWordWrapMode(QTextOption::WrapAnywhere);
	_browser->setAutoFormatting(QTextEdit::AutoAll);
	new CPCopyContextMenu(_browser); //自定义汉化右键菜单

	QWidget* rightTopContainer = new QWidget();
	rightTopContainer->setObjectName("right_top_container");
	QHBoxLayout* rightTopLayout = new QHBoxLayout();
	rightTopLayout->setContentsMargins(10, 5, 10, 5);

	CPToolButton::BtnConfig exportConfig;
	exportConfig.icon_normal = ":/styles/icons/report/export.png";
	exportConfig.icon_hover = ":/styles/icons/report/export_hover.png";
	exportConfig.icon_disable = exportConfig.icon_normal;
	exportConfig.text = tr("export report");
	CPToolButton* exportButton = new CPToolButton(exportConfig);
	exportButton->setObjectName("report_button");

	CPToolButton::BtnConfig refConfig;
	refConfig.icon_normal = ":/styles/icons/report/refresh.png";
	refConfig.icon_hover = ":/styles/icons/report/refresh_hover.png";
	refConfig.icon_disable = refConfig.icon_normal;
	refConfig.text = tr("refresh");
	_refreshButton = new CPToolButton(refConfig);
	_refreshButton->setObjectName("report_button");

	rightTopLayout->addWidget(exportButton);
	rightTopLayout->addWidget(_refreshButton);
	rightTopLayout->addStretch(1);

	_searchEdit = new CPSearchEdit();
	_searchEdit->setTimerInterval(200);
	rightTopLayout->addWidget(_searchEdit);
	rightTopContainer->setLayout(rightTopLayout);

	QLayout* layout = CPPageFactory::createTreeTablePage(_treeWidget, _browser, rightTopContainer);
	setLayout(layout);

	_rootRecord = CPCase::get()->rootRecord()->getType()->newRecord();

	connect(_refreshButton, SIGNAL(clicked()), this, SLOT(slotRefreshClicked()));
	connect(_searchEdit, SIGNAL(signalTextChanged(const QString&)), this,
		SLOT(slotSearchTextChanged(const QString&)));
	connect(_searchEdit, SIGNAL(signalEnterPressed()), this,
		SLOT(slotSearchEnterPressed()));
	connect(exportButton, SIGNAL(clicked()), this, SLOT(slotExportClicked()));

	connect(CPReportStatus::get(), SIGNAL(signalRecordChanged()), this, SLOT(slotRecordChanged()));
	CPReportCreater2::get(this)->addEternalFinishCallback([this](){
		slotReportCreated();
	});
	connect(_treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
		this, SLOT(slotItemClicked(QTreeWidgetItem *, int)));

	this->setDisabled(true);
	CPCase::get()->addCreateCallback([this](){
		this->setDisabled(false);
		slotRecordChanged();
	});
}

void CPReportView::slotRecordChanged()
{
	std::set<int> uncheckedIds;
	//获取选中的节点id
	CPReportRecordHandler::getUnCheckedIds(_rootRecord, uncheckedIds);
	_rootRecord->clearChilren();
	CPRecord* rootRecord = CPCase::get()->rootRecord();
	//重新拷贝树
	siutil::CPRecordHandler::copyTree(rootRecord, _rootRecord, RFlagNoShow);
	_treeWidget->clear();
	for (CPRecord* child : _rootRecord->getChildren()) {
		QTreeWidgetItem* item = _treeWidget->addTreeItem(child, nullptr);
		_treeWidget->expandItem(item);
	}
	//恢复之前选中的节点
	_rootRecord->setChecked(true);
	CPReportRecordHandler::uncheckAppsById(uncheckedIds, _rootRecord);
	_treeWidget->updateItemCheckState();
}


void CPReportView::startCreateReport()
{
	if (_rootRecord->getCheckState() == RECORD_CHECK_NONE) {
		return;
	}

	this->setDisabled(true);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	qApp->processEvents();

	CPRecord* rootRecord = CPCase::get()->rootRecord();
	//把未选中的节点，添加RFlagNoReport；选中的节点去掉RFlagNoReport
	siutil::CPRecordHandler::setCheckedFlags(_rootRecord, rootRecord, RFlagNoReport, false);
	CPReportCreater2::get()->createReport(rootRecord);
}

void CPReportView::slotReportCreated()
{
	this->setDisabled(false);
	QApplication::restoreOverrideCursor();
	qApp->processEvents();

	if (_startType == StartByRefresh) {
		loadReport();
	}
	else if (_startType == StartByExport) {
		doExportReport();
	}
}

void CPReportView::slotRefreshClicked()
{
#if 0
	//测试用
	loadReport();
	return;
#endif
	_startType = StartByRefresh;
	startCreateReport();
}

void CPReportView::loadReport()
{
	_browser->clear();
	std::wstring htmlPath = CPReportCreater2::get()->htmlPath();
	if (!htmlPath.empty()) {
		std::wstring htmlDir = CPPath::getDir(htmlPath);
		//设置搜索的路径和图片的路径
		_browser->setSearchPaths(QStringList() << QString::fromStdWString(htmlDir));
		_browser->setSource(ContentHtml);
	}
	slotItemClicked(_treeWidget->currentItem(), 0);
}

void CPReportView::slotSearchTextChanged(const QString& text)
{
	_browser->find(text);
}

void CPReportView::slotSearchEnterPressed()
{
	QString text = _searchEdit->text().trimmed();
	bool ret = _browser->find(text);
	if (!ret) {
		//如果没有找到，则回到开始重新找
		QTextCursor curPos = _browser->textCursor();
		QTextCursor beginPos = curPos;
		beginPos.movePosition(QTextCursor::Start);
		_browser->setTextCursor(beginPos);
		if (!_browser->find(text)) {
			_browser->setTextCursor(curPos);
		}
	}
}

void CPReportView::slotItemClicked(QTreeWidgetItem *item, int column)
{
	CPRecordItemWidget* widget = dynamic_cast<CPRecordItemWidget*>
		(_treeWidget->itemWidget(item, 0));
	if (!widget) {
		return;
	}
	CPRecord* node = widget->node();
	if (!node) {
		return;
	}
	//和报告中的ID产生方法，保持一致
	QString archorName = QString::fromStdString(IdPre + I2S(node->id()));
	QUrl url = ContentHtml + "#" + archorName;
	_browser->setSource(url);
}

void CPReportView::slotExportClicked()
{
	_startType = StartByExport;
	startCreateReport();
}

void CPReportView::doExportReport()
{
	CPExportReportDialog dialog(this);
	std::wstring reportDir = CPReportCreater2::get()->reportDir();
	dialog.setDir(reportDir);
	int ret = dialog.exec();
	if (ret != CPDialog::Accepted) {
		return;
	}
	std::wstring exportDir = dialog.getDir();
	//检测路径和可用空间
	const uint MinSize = 1024 * 1024;
	bool bret = CPFilePathChecker::checkTargetPath(exportDir,
		MinSize, MinSize);
	if (!bret) {
		return;
	}
	//在选择的导出目录后面，添加报告目录
	std::wstring dirName = CPPath::getFileName(exportDir);
	if (dirName != ReportDirName) {
		exportDir += L"/" + ReportDirName;
	}
	if (siutil::CPDevicePath::isSubPath(reportDir, exportDir)) {
		//和默认生成的报告目录相同
		CPMessageBox::info(tr("export report success."));
		return;
	}
	bret = CPPath::copyFiles(reportDir, exportDir);
	if (bret) {
		CPMessageBox::info(tr("export report success."));
	}
	else {
		CPMessageBox::warning(tr("export report failed!"));
	}
}