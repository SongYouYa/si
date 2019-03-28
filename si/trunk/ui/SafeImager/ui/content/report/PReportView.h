#pragma once
#include "uicommon/record/PAppTreeWidget.h"
#include "ui/common/edit/PSearchEdit.h"

class CPReportView : public QWidget
{
	Q_OBJECT
public:
	CPReportView(QWidget* parent = nullptr);

	private slots:
	//导出报告
	void slotExportClicked();
	//刷新报告
	void slotRefreshClicked();
	//搜索
	void slotSearchTextChanged(const QString&);
	void slotSearchEnterPressed();
	//报告数据有变化
	void slotRecordChanged();
	//树节点点击
	void slotItemClicked(QTreeWidgetItem *item, int column);
	//报告生成
	void slotReportCreated();

private:
	//开始生成报告
	void startCreateReport();
	//预览加载报告
	void loadReport();
	//导出报告
	void doExportReport();

	CPAppTreeWidget* _treeWidget;
	CPToolButton* _refreshButton;
	CPSearchEdit* _searchEdit;
	QTextBrowser* _browser;

	CPRecord* _rootRecord;

	enum StartType
	{
		StartByNone,
		StartByRefresh,
		StartByExport
	};
	StartType _startType;
};