#include "stdafx.h"
#include "PDoneTaskListWidget.h"
#include "./detail/PDetailButtonFactory.h"
#include "./PDoneTaskItemWidget.h"

CPDoneTaskListWidget::CPDoneTaskListWidget(QWidget* parent)
	: CPTaskListWidget(parent)
{

}

CPTaskItemWidgetBase* CPDoneTaskListWidget::newItemWidget(CPTask* task)
{
	QPushButton *detailButton = CPDetailButtonFactory::createButton(task);
	auto itemWidget = new CPDoneTaskItemWidget(detailButton);
	itemWidget->setTask(task);
	return itemWidget;
}