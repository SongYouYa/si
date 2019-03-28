#pragma once
#include "./PTaskItemWidgetBase.h"

class CPTaskTreeWidgetItem : public QTreeWidgetItem
{
public:
	bool operator<(const QTreeWidgetItem& other) const override;
};

class CPTaskListWidget : public QTreeWidget
{
	Q_OBJECT
public:
	CPTaskListWidget(QWidget* parent = nullptr);

	void addTask(CPTask* task);

protected:
	virtual CPTaskItemWidgetBase* newItemWidget(CPTask* task) = 0;
	void deleteTaskItem(QTreeWidgetItem*);
	virtual void onNewTaskItem(CPTaskItemWidgetBase* itemWidget, QTreeWidgetItem* item) {};

	void doDeleteItem(QTreeWidgetItem* item);

signals:
	void signalTaskDeleted(CPTask*);

};