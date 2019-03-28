#pragma once
#include "./PTaskItemWidgetBase.h"
#include "uicommon/widget/PTextEdit.h"

class CPTaskLogDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPTaskLogDialog(QWidget* parent = nullptr);

	void setTask(CPTask* task);
private:
	CPTextEdit* _textEdit;
};

class CPDoneTaskItemWidget : public CPTaskItemWidgetBase
{
	Q_OBJECT
public:
	CPDoneTaskItemWidget(QPushButton *detailButton, QWidget* parent = nullptr);

	void setTask(CPTask* task) override;

private:
	QString getStatusText(TASK_STATUS status, QString& labelCss);

	QLabel* _statusTextLabel;
	QPushButton* _resultButton;
};
