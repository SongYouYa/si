#include "stdafx.h"
#include "PDoneTaskItemWidget.h"


CPTaskLogDialog::CPTaskLogDialog(QWidget* parent)
	: CPDialog2(parent)
{
	setWindowTitle(tr("task log"));
	setCancelHidden(true);
	enableResizer(true);
	resize(QSize(565, 320));

	QWidget* content = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	_textEdit = new CPTextEdit();
	_textEdit->setReadOnly(true);
	//_textEdit->setWordWrap(true);
	layout->addWidget(_textEdit);
	content->setLayout(layout);

	setContentWidget(content);
	setupButtons();
}

void CPTaskLogDialog::setTask(CPTask* task)
{
	std::wstring strLog;
	CPTaskFactory* taskFactory = CPTaskController::get()->getTaskFactory();
	taskFactory->getTaskLog(task, strLog);
	_textEdit->setText(QString::fromStdWString(strLog));
}

CPDoneTaskItemWidget::CPDoneTaskItemWidget(QPushButton *detailButton,
	QWidget* parent)
	: CPTaskItemWidgetBase(parent)
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(15, 5, 10, 5);
	layout->setSpacing(10);

	_sizeSlashLabel->setVisible(false);
	_totalSizeLabel->setVisible(false);
	layout->addWidget(_titleContainer);

	layout->addStretch(2);

	layout->addWidget(_statusIconLabel);
	_statusTextLabel = new QLabel();
	layout->addWidget(_statusTextLabel);

	layout->addStretch(1);

	//_usedTimeTitle->setText(tr("total time:"));
	QHBoxLayout* timeLayout = new QHBoxLayout();
	timeLayout->setSpacing(0);
	timeLayout->addWidget(_usedTimeTitle);
	timeLayout->addWidget(_usedTimeValue);
	layout->addLayout(timeLayout);

	layout->addStretch(1);

	_resultButton = new QPushButton();
	_resultButton->setObjectName("task_log");
	_resultButton->setToolTip(tr("task log"));
	layout->addWidget(_resultButton);
	layout->addWidget(_deleteButton);
	if (detailButton) {
		layout->addWidget(detailButton);
	}

	layout->addStretch(1);

	setLayout(layout);
}

QString CPDoneTaskItemWidget::getStatusText(TASK_STATUS status, QString& labelCss)
{
	labelCss = "color: green;";
	QString strStatus;
	switch (status)
	{
	/*case TASK_STATUS_INIT:
		break;
	case TASK_STATUS_WAITING:
		break;
	case TASK_STATUS_PAUSED:
		break;
	case TASK_STATUS_RUNNING:
		break;*/
	case TASK_STATUS_FINISHED:
		strStatus = tr("task_finished");
		break;
	case TASK_STATUS_FAILED:
		strStatus = tr("task_failed");
		labelCss = "color: red;";
		break;
	/*case TASK_STATUS_CANCELED:
		break;*/
	default:
		assert(false);
		break;
	}
	return strStatus;
}

void CPDoneTaskItemWidget::setTask(CPTask* task)
{
	CPTaskItemWidgetBase::setTask(task);
	CPTaskStatusCtrl* statusCtrl = task->statusCtrl();
	CPTaskProgress* progress = statusCtrl->progress();

	std::wstring strCurSize = CPUtils::formatSizeUnit(progress->currentSize());
	_curSizeLabel->setText(QString::fromStdWString(strCurSize));

	QString labelCss;
	QString strStatus = getStatusText(statusCtrl->status(), labelCss);
	_statusTextLabel->setText(strStatus);
	_statusTextLabel->setStyleSheet(labelCss);

	std::string strUsedTime = CPUtils::formatTimePeriod(progress->getUsedTime() / 1000);
	_usedTimeValue->setText(QString::fromStdString(strUsedTime));

	connect(_resultButton, &QPushButton::clicked, this, [task](){
		CPTaskLogDialog dialog;
		dialog.setTask(task);
		dialog.exec();
	});
}