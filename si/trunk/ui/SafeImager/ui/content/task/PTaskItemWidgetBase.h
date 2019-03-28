#pragma once
#include "uicommon/task/PTaskStatusHandler.h"

class CPTaskItemWidgetBase : public QWidget
{
	Q_OBJECT
public:
	CPTaskItemWidgetBase(QWidget* parent);

	virtual void setTask(CPTask* task);

	CPTask* task() {
		return _task;
	}

signals:
	void signalDeleteItem();
	void signalStatusChanged(TASK_STATUS curStatus);

protected:
	CPTask* _task;
	TaskCfg _taskCfg;

	QWidget* _titleContainer;
	QLabel* _titleIconLabel;
	QLabel* _titleTextLabel;

	QWidget* _sizeContainer;
	QLabel* _curSizeLabel;
	QLabel* _totalSizeLabel;
	QLabel* _sizeSlashLabel;

	QLabel* _usedTimeTitle;
	QLabel* _usedTimeValue;

	QPushButton* _deleteButton;

	QLabel* _statusIconLabel;
	CPTaskStatusHandler* _statusHandler;
};
