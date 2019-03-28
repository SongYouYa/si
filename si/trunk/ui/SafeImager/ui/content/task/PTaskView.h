#pragma once

class CPVTab2ItemWidget;
class CPTaskTabDecorator;
class CPRunningTaskListWidget;
class CPTaskView : public QWidget
{
	Q_OBJECT
public:
	CPTaskView(QWidget* parent = nullptr);

	void decorateTab(QWidget* tabButton);

private:
	void onCreateTask(CPTask* task, bool display);
	//更新显示进行任务的数量图标
	void updateRunningCount();

	CPRunningTaskListWidget* _runningListWidget;
	CPTaskTabDecorator* _tabDecorator;

	CPVTab2ItemWidget* _runningTab;
	CPVTab2ItemWidget* _doneTab;
};