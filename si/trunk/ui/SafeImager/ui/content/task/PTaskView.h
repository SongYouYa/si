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
	//������ʾ�������������ͼ��
	void updateRunningCount();

	CPRunningTaskListWidget* _runningListWidget;
	CPTaskTabDecorator* _tabDecorator;

	CPVTab2ItemWidget* _runningTab;
	CPVTab2ItemWidget* _doneTab;
};