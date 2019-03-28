#include "stdafx.h"
#include "PRunningTaskItemWidget.h"
#include "task/PTaskCfg.h"
#include "uicommon/widget/PTextEdit.h"


CPRunningTaskItemWidget::CPRunningTaskItemWidget(QPushButton *detailButton, 
	QWidget* parent)
	: CPTaskItemWidgetBase(parent),
_progressHandler(nullptr),
_progressBar(nullptr),
_pauseButton(nullptr),
_continueButton(nullptr)
{
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(15, 5, 10, 5);
	layout->setSpacing(15);

	layout->addWidget(_titleContainer);

	QVBoxLayout* centerLayout = new QVBoxLayout();
	centerLayout->setSpacing(8);
	centerLayout->setMargin(0);
	centerLayout->addStretch();
	_progressBar = new QProgressBar();
	_progressBar->setMaximumHeight(8);
	_progressBar->setTextVisible(false);
	centerLayout->addWidget(_progressBar);
	
	QHBoxLayout* speedTimeLayout = new QHBoxLayout();
	speedTimeLayout->setMargin(0);
	speedTimeLayout->setSpacing(0);

	_speedWidget = new QStackedWidget();
	QWidget* speedContainer = new QWidget();
	QHBoxLayout* speedLayout = new QHBoxLayout();
	speedLayout->setMargin(0);
	speedLayout->setSpacing(0);
	_speedTitle = new QLabel(tr("speed:"));
	speedLayout->addWidget(_speedTitle);
	_speedLabel = new QLabel();
	speedLayout->addWidget(_speedLabel);
	speedContainer->setLayout(speedLayout);
	_speedWidget->addWidget(speedContainer);

	_speedWidget->addWidget(new QWidget());
	speedTimeLayout->addWidget(_speedWidget);

	speedTimeLayout->addStretch();

	speedTimeLayout->addWidget(_usedTimeTitle);
	speedTimeLayout->addWidget(_usedTimeValue);

	speedTimeLayout->addStretch();

	QLabel* leftTimeTitle = new QLabel(tr("left time:"));
	QLabel* leftTimeValue = new QLabel("--:--:--");
	speedTimeLayout->addWidget(leftTimeTitle);
	speedTimeLayout->addWidget(leftTimeValue);

	centerLayout->addLayout(speedTimeLayout);
	centerLayout->addStretch();
	layout->addLayout(centerLayout, 15);

	layout->addStretch(2);

	QHBoxLayout* rightLayout = new QHBoxLayout();
	_pauseButton = new QPushButton();
	_pauseButton->setObjectName("task_pause");
	_pauseButton->setToolTip(tr("pause"));
	rightLayout->addWidget(_pauseButton);

	_continueButton = new QPushButton();
	_continueButton->setObjectName("task_continue");
	_continueButton->setToolTip(tr("continue"));
	_continueButton->setVisible(false);
	rightLayout->addWidget(_continueButton);
	rightLayout->addWidget(_deleteButton);

	if (detailButton) {
		rightLayout->addWidget(detailButton);
	}
	layout->addLayout(rightLayout);
	layout->addStretch(1);
	setLayout(layout);

	_progressHandler = new CPTaskProgressHandler(this, _progressBar,
		_curSizeLabel, _totalSizeLabel, _usedTimeValue, leftTimeValue, _speedLabel);
	_progressHandler->setSizeFormat(SIZE_F_UNIT);

	connect(_pauseButton, &QPushButton::clicked, this, [this](){
		if (_task) {
			_task->statusCtrl()->togglePause();
		}
	});
	connect(_continueButton, &QPushButton::clicked, this, [this](){
		if (_task) {
			_task->statusCtrl()->togglePause();
		}
	});

	connect(_statusHandler, &CPTaskStatusHandler::qsignalStatusChanged, this, 
		[this](TASK_STATUS cur){
		_continueButton->setVisible(cur == TASK_STATUS_PAUSED);
		_pauseButton->setVisible(!_continueButton->isVisible());
	});
}

CPRunningTaskItemWidget::~CPRunningTaskItemWidget()
{

}

void CPRunningTaskItemWidget::setTask(CPTask* task)
{
	CPTaskItemWidgetBase::setTask(task);
	_progressHandler->setProgress(task->statusCtrl()->progress());
	if (!_taskCfg.withSpeed) {
		_speedWidget->setCurrentIndex(1);
	}
}
