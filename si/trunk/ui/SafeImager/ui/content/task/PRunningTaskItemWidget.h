#pragma once
#include "./PTaskItemWidgetBase.h"
#include "uicommon/task/PTaskProgressHandler.h"

class CPRunningTaskItemWidget : public CPTaskItemWidgetBase
{
	Q_OBJECT
public:
	//isDetail���Ƿ�������ϸ��Ϣҳ�洴���ģ���Ҫ���������ߵİ�ť��ʾ��ͬ
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
