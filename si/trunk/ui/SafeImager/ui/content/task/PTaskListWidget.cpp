#include "stdafx.h"
#include "PTaskListWidget.h"

#include "uicommon/task/PTaskCallbackHandler.h"
#include "task/PTaskCfg.h"

#include "./PTaskHandlers.h"

enum PTaskDataRole
{
	PTaskSortRole = Qt::UserRole + 1
};

bool CPTaskTreeWidgetItem::operator<(const QTreeWidgetItem& other) const
{
	int value = data(0, PTaskSortRole).toInt();
	int ovalue = other.data(0, PTaskSortRole).toInt();
	return value < ovalue;
}

CPTaskListWidget::CPTaskListWidget(QWidget* parent)
: QTreeWidget(parent)
{
	setObjectName("tasklist");
	setStyleSheet(".QTreeWidget{border: none;}");
	setIndentation(0);
	setHeaderHidden(true);
}

void CPTaskListWidget::addTask(CPTask* task)
{
	CPTaskItemWidgetBase* itemWidget = newItemWidget(task);

	QTreeWidgetItem* item = new CPTaskTreeWidgetItem();
	//逆序排列
	item->setData(0, PTaskSortRole, topLevelItemCount());
	addTopLevelItem(item);
	sortItems(0, Qt::DescendingOrder);
	setItemWidget(item, 0, itemWidget);

	connect(itemWidget, &CPTaskItemWidgetBase::signalDeleteItem, this, [this, item](){
		deleteTaskItem(item);
	});
	onNewTaskItem(itemWidget, item);
}

void CPTaskListWidget::deleteTaskItem(QTreeWidgetItem* item)
{
	if (CPMessageBox::question(tr("Sure to delete the task?"))
		!= CPDialog::Accepted) {
		return;
	}

	doDeleteItem(item);
}

void CPTaskListWidget::doDeleteItem(QTreeWidgetItem* item)
{
	QWidget* widget = itemWidget(item, 0);
	if (auto itemWidget = dynamic_cast<CPTaskItemWidgetBase*>(widget)) {
		CPTask* task = itemWidget->task();
		auto statusCtrl = task->statusCtrl();
		if (statusCtrl->isRunning()) {
			statusCtrl->cancel();
		}
#if 0
		//这里不再删除handler，因为其他地方，可能会用到
		CPTaskHandlers::get()->delTask(task);
#endif
		delete item;
		emit signalTaskDeleted(task);
	}
	else {
		emit signalTaskDeleted(nullptr);
	}
}