#pragma once
#include "uicommon/record/PAppTreeWidget.h"
#include "ui/common/edit/PSearchEdit.h"

class CPReportView : public QWidget
{
	Q_OBJECT
public:
	CPReportView(QWidget* parent = nullptr);

	private slots:
	//��������
	void slotExportClicked();
	//ˢ�±���
	void slotRefreshClicked();
	//����
	void slotSearchTextChanged(const QString&);
	void slotSearchEnterPressed();
	//���������б仯
	void slotRecordChanged();
	//���ڵ���
	void slotItemClicked(QTreeWidgetItem *item, int column);
	//��������
	void slotReportCreated();

private:
	//��ʼ���ɱ���
	void startCreateReport();
	//Ԥ�����ر���
	void loadReport();
	//��������
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