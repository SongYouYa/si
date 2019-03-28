#pragma once
#include "./PTaskItemWidgetBase.h"
#include "uicommon/task/PTaskProgressHandler.h"

class CPRunningTaskItemWidget : public CPTaskItemWidgetBase
{
	Q_OBJECT
public:
	//isDetail：是否是在详细信息页面创建的，主要是左右两边的按钮显示不同
	CPRunningTaskItemWidget(QPushButton *detailButton,
		QWidget* parent = nullptr);
	~CPRunningTaskItemWidget();

	void setTask(CPTask* task) override;

private:
	CPTaskProgressHandler* _progressHandler;
	QProgressBar* _progressBar;

	QStackedWidget* _speedWidget;
	QLabel* _speedTitle;
	QLabel* _speedLabel;
	QPushButton* _pauseButton;
	QPushButton* _continueButton;
};
