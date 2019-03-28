#include "stdafx.h"
#include "PTaskItemWidgetBase.h"


CPTaskItemWidgetBase::CPTaskItemWidgetBase(QWidget* parent)
	: QWidget(parent),
	_task(nullptr)
{
	setFixedHeight(62);

	_titleContainer = new QWidget();
	_titleContainer->setFixedWidth(165);
	QHBoxLayout* titleLayout = new QHBoxLayout();
	titleLayout->setSpacing(15);
	_titleIconLabel = new QLabel();
	titleLayout->addWidget(_titleIconLabel);

	QVBoxLayout* titleRightLayout = new QVBoxLayout();
	titleRightLayout->setSpacing(0);

	titleRightLayout->addStretch();

	_titleTextLabel = new QLabel();
	_titleTextLabel->setObjectName("title1_label");
	titleRightLayout->addWidget(_titleTextLabel);

	_sizeContainer = new QWidget();
	QHBoxLayout* sizeLayout = new QHBoxLayout();
	sizeLayout->setMargin(0);
	sizeLayout->setSpacing(0);
	_curSizeLabel = new QLabel("0");
	_totalSizeLabel = new QLabel("0");
	_sizeSlashLabel = new QLabel("/");
	sizeLayout->addWidget(_curSizeLabel);
	sizeLayout->addWidget(_sizeSlashLabel);
	sizeLayout->addWidget(_totalSizeLabel);
	_sizeContainer->setLayout(sizeLayout);
	titleRightLayout->addWidget(_sizeContainer);

	titleRightLayout->addStretch();

	titleLayout->addLayout(titleRightLayout);
	titleLayout->addStretch();
	_titleContainer->setLayout(titleLayout);

	_usedTimeTitle = new QLabel(tr("usded time:"));
	_usedTimeValue = new QLabel("--:--:--");

	_statusIconLabel = new QLabel();

	_deleteButton = new QPushButton();
	_deleteButton->setObjectName("task_delete");
	_deleteButton->setToolTip(tr("delete"));

	_statusHandler = new CPTaskStatusHandler(this, _statusIconLabel,
		nullptr);
	connect(_statusHandler, &CPTaskStatusHandler::qsignalStatusChanged, this, [this](TASK_STATUS curStatus){
		emit signalStatusChanged(curStatus);
	}, Qt::QueuedConnection);

	connect(_deleteButton, &QPushButton::clicked, this, [this](){
		emit signalDeleteItem();
	});
}

void CPTaskItemWidgetBase::setTask(CPTask* task)
{
	_task = task;
	_taskCfg = CPTaskCfg::get()->getTaskCfg(task->type());
	_statusHandler->setTask(task);
	_titleTextLabel->setText(QString::fromStdWString(_taskCfg.display));
	_titleIconLabel->setPixmap(QPixmap(":/styles/icons/task/type/" + 
		QString::fromStdWString(_taskCfg.icon)));
	if (!_taskCfg.withSpeed) {
		_sizeContainer->setVisible(false);
	}
}