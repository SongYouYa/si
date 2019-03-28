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
	new CPCopyContextMenu(_browser); //�Զ��庺���Ҽ��˵�

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
	//��ȡѡ�еĽڵ�id
	CPReportRecordHandler::getUnCheckedIds(_rootRecord, uncheckedIds);
	_rootRecord->clearChilren();
	CPRecord* rootRecord = CPCase::get()->rootRecord();
	//���¿�����
	siutil::CPRecordHandler::copyTree(rootRecord, _rootRecord, RFlagNoShow);
	_treeWidget->clear();
	for (CPRecord* child : _rootRecord->getChildren()) {
		QTreeWidgetItem* item = _treeWidget->addTreeItem(child, nullptr);
		_treeWidget->expandItem(item);
	}
	//�ָ�֮ǰѡ�еĽڵ�
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
	//��δѡ�еĽڵ㣬���RFlagNoReport��ѡ�еĽڵ�ȥ��RFlagNoReport
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
	//������
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
		//����������·����ͼƬ��·��
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
		//���û���ҵ�����ص���ʼ������
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
	//�ͱ����е�ID��������������һ��
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
	//���·���Ϳ��ÿռ�
	const uint MinSize = 1024 * 1024;
	bool bret = CPFilePathChecker::checkTargetPath(exportDir,
		MinSize, MinSize);
	if (!bret) {
		return;
	}
	//��ѡ��ĵ���Ŀ¼���棬��ӱ���Ŀ¼
	std::wstring dirName = CPPath::getFileName(exportDir);
	if (dirName != ReportDirName) {
		exportDir += L"/" + ReportDirName;
	}
	if (siutil::CPDevicePath::isSubPath(reportDir, exportDir)) {
		//��Ĭ�����ɵı���Ŀ¼��ͬ
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