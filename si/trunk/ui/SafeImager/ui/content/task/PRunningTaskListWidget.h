#pragma once
#include "./PTaskListWidget.h"

class CPRunningTaskListWidget : public CPTaskListWidget
{
	Q_OBJECT
public:
	CPRunningTaskListWidget(QWidget* parent = nullptr);

signals:
	void signalTaskEnded(CPTask* task);

protected:
	CPTaskItemWidgetBase* newItemWidget(CPTask* task) override;

	void onNewTaskItem(CPTaskItemWidgetBase* itemWidget, QTreeWidgetItem* item) override;
};
