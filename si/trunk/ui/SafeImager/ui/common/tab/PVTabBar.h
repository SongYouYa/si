#pragma once
#include "uicommon/tab/PTabBar.h"
#include "./PVTabItemWidget.h"

class CPVTabBar : public CPTabBar
{
	Q_OBJECT
public:
	CPVTabBar(const QString& configPath, QWidget* parent = nullptr);

	void setMinWidth(int width);

protected:
	virtual QWidget* newItemWidget(const CPVTabItemWidget::Config& config);
	QWidget* addTab(const QString& tabName, const QString& displayName);
	void endArea();
	void setCurTab(const QString& tabName);

	protected slots:
	virtual void slotItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);

protected:
	QTreeWidget* _treeWidget;
	std::map<QString, QTreeWidgetItem*> _tabItems;
};