#include "stdafx.h"
#include "PTaskView.h"
#include "PTaskListWidget.h"

#include "ui/common/tab/PLevel2TabBar.h"

#include "./PTaskHandlers.h"
#include "PRunningTaskListWidget.h"
#include "PDoneTaskListWidget.h"
#include "./material/PTaskTabDecorator.h"

#include "logic/PReportStatus.h"

#define TASK_TAB_DOING "tab_task_doing"
#define TASK_TAB_DONE "tab_task_done"

CPTaskView::CPTaskView(QWidget* parent)
: QWidget(parent),
_tabDecorator(new CPTaskTabDecorator(this))
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	CPLevel2TabBar* tabBar = new CPLevel2TabBar(":/styles/config/tasktab.json");
	layout->addWidget(tabBar);

	QStackedWidget* stackedWidget = new QStackedWidget();
	layout->addWidget(stackedWidget);

	CPTabManager* taskTab = new CPTabManager(tabBar, stackedWidget);

	_runningListWidget = new CPRunningTaskListWidget();
	QWidget* runningButton = taskTab->addTab(TASK_TAB_DOING, dtr(TASK_TAB_DOING), _runningListWidget);
	_runningTab = qobject_cast<CPVTab2ItemWidget*>(runningButton);

	CPDoneTaskListWidget* doneList = new CPDoneTaskListWidget();
	QWidget* doneButton = taskTab->addTab(TASK_TAB_DONE, dtr(TASK_TAB_DONE), doneList);
	_doneTab = qobject_cast<CPVTab2ItemWidget*>(doneButton);

	setLayout(layout);

	//任务完成后，加入任务完成列表
	connect(_runningListWidget, &CPRunningTaskListWidget::signalTaskEnded, this, [this, doneList](CPTask* task){
		doneList->addTask(task);
		_doneTab->setCount(doneList->topLevelItemCount());
		updateRunningCount();
		CPReportStatus::get()->sendRecordChanged();
	});
	connect(_runningListWidget, &CPTaskListWidget::signalTaskDeleted, this, [this](CPTask* task) {
		updateRunningCount();
	});
	connect(doneList, &CPTaskListWidget::signalTaskDeleted, this, [this, doneList](CPTask* task) {
		_doneTab->setCount(doneList->topLevelItemCount());
	});

	CPTaskHandlers::get(this);

	CPTaskController::get()->addCreateCallback(
		std::bind(&CPTaskView::onCreateTask, this, std::placeholders::_1,
		std::placeholders::_2));

	taskTab->navToTab(TASK_TAB_DOING);

	//初始化
	updateRunningCount();
	_doneTab->setCount(doneList->topLevelItemCount());
}

void CPTaskView::decorateTab(QWidget* tabButton)
{
	_tabDecorator->decorate(tabButton);
	updateRunningCount();
}

void CPTaskView::onCreateTask(CPTask* task, bool display)
{
	CPTaskHandlers::get()->addTask(task);
	if (display) {
		_runningListWidget->addTask(task);
		updateRunningCount();
	}
}

void CPTaskView::updateRunningCount()
{
	int runningCount = _runningListWidget->topLevelItemCount();
	_tabDecorator->setTaskCount(runningCount);
	_runningTab->setCount(runningCount);
}
