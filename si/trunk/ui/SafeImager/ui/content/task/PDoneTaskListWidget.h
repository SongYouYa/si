#pragma once
#include "./PTaskListWidget.h"


class CPDoneTaskListWidget : public CPTaskListWidget
{
	Q_OBJECT
public:
	CPDoneTaskListWidget(QWidget* parent = nullptr);

protected:
	CPTaskItemWidgetBase* newItemWidget(CPTask* task) override;
};
