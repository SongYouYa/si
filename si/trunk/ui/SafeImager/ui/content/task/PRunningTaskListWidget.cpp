#include "stdafx.h"
#include "PRunningTaskListWidget.h"
#include "./detail/PDetailButtonFactory.h"
#include "PRunningTaskItemWidget.h"

CPRunningTaskListWidget::CPRunningTaskListWidget(QWidget* parent)
	: CPTaskListWidget(parent)
{

}

CPTaskItemWidgetBase* CPRunningTaskListWidget::newItemWidget(CPTask* task)
{
	QPushButton *detailButton = CPDetailButtonFactory::createButton(task);
	auto itemWidget = new CPRunningTaskItemWidget(detailButton);
	itemWidget->setTask(task);
	return itemWidget;
}

void CPRunningTaskListWidget::onNewTaskItem(CPTaskItemWidgetBase* itemWidget, QTreeWidgetItem* item)
{
	CPTask* task = itemWidget->task();
	task->addUiEndCallback([this, item](CPTask* task){
		//��������󣬴����ڽ����б���ɾ��
		doDeleteItem(item);
		if (!task->statusCtrl()->isCanceled()) {
			emit signalTaskEnded(task);
		}
	});
}