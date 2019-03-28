#include "stdafx.h"
#include "PMemoryProgressDialog.h"

#include "ui/common/widget/PDirSelectWidget.h"
#include "ui/common/widget/PWidgetTitleWrapper.h"

CPMemoryProgressDialog::CPMemoryProgressDialog(QWidget* parent)
	: CPDialog2(parent)
{
	setWindowTitle(tr("create image"));
	enableResizer(true);
	resize(QSize(440, 320));
	setOkButtonText(tr("finish"));

	QWidget* content = new QWidget();
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setContentsMargins(20, 10, 20, 10);
	layout->setSpacing(5);

	_imageDirWidget = new CPDirSelectWidget();
	auto dirWrapper = new CPWidgetTitleWrapper(_imageDirWidget);
	dirWrapper->setTitle(tr("save dir"));
	_imageDirWidget->setDisabled(true);
	_imageDirWidget->setMaximumWidth(500);
	layout->addWidget(dirWrapper);

	_filenameEdit = new CPFileNameEdit();
	auto fileWrapper = new CPWidgetTitleWrapper(_filenameEdit);
	fileWrapper->setTitle(tr("filename"));
	_filenameEdit->setMaximumWidth(500);
	_filenameEdit->setDisabled(true);
	layout->addWidget(fileWrapper);

	layout->addSpacing(10);

	QWidget* progressContainer = new QWidget();
	QVBoxLayout* progressLayout = new QVBoxLayout();
	progressLayout->setMargin(0);
	QProgressBar* progressBar = new QProgressBar();
	progressBar->setTextVisible(false);
	progressBar->setFixedHeight(16);
	progressBar->setObjectName("momory_progress");
	progressLayout->addWidget(progressBar);

	QHBoxLayout* timeLayout = new QHBoxLayout();
	timeLayout->setMargin(0);
	timeLayout->setSpacing(5);
	QLabel* usedTimeTitle = new QLabel(tr("used time:"));
	QLabel* usedTimeLabel = new QLabel("--:--:--");
	QLabel* leftTimeTitle = new QLabel(tr("left time:"));
	QLabel* leftTimeLabel = new QLabel("--:--:--");
	timeLayout->addWidget(usedTimeTitle);
	timeLayout->addWidget(usedTimeLabel);
	timeLayout->addStretch();
	timeLayout->addWidget(leftTimeTitle);
	timeLayout->addWidget(leftTimeLabel);
	progressLayout->addLayout(timeLayout);

	progressContainer->setLayout(progressLayout);

	auto progressWrapper = new CPWidgetTitleWrapper(progressContainer);
	progressWrapper->setTitle(tr("creating image..."));
	layout->addWidget(progressWrapper);

	content->setLayout(layout);
	setContentWidget(content);
	setupButtons();

	_progressHandler = new CPTaskProgressHandler(this, progressBar,
		nullptr, nullptr, usedTimeLabel, leftTimeLabel, nullptr);

	setOkButtonDisabled(true);
}

void CPMemoryProgressDialog::setTask(CPTask* task)
{
	_task = task;
	_callbackHandler = new CPTaskCallbackHandler(task, this);
	_statusHandler = new CPTaskStatusHandler(this);
	_statusHandler->setTask(task);
	_progressHandler->setProgress(task->statusCtrl()->progress());

	_task->addUiEndCallback([this](CPTask* task) {
		CPTaskStatusCtrl* statusCtrl = _task->statusCtrl();
		if (statusCtrl->isFailed()) {
			CPMessageBox::warning(tr("Memory image task failed! Please close the anti-virus softwares and retry."), this);
			this->done(CPDialog::Accepted);
		}
		else if (statusCtrl->isCanceled()) {
			this->done(CPDialog::Accepted);
		}
		else {
			this->setOkButtonDisabled(false);
			this->setCancelButtonDisabled(true);
			CPMessageBox::info(tr("Memory image task success."), this);
		}
	});
}

void CPMemoryProgressDialog::closeEvent(QCloseEvent* e)
{
	int ret = CPMessageBox::question(tr("stop image task?"));
	if (ret == CPDialog::Accepted) {
		_task->statusCtrl()->cancel();
		CPDialog::closeEvent(e);
	}
	else {
		e->ignore();
	}
}

void CPMemoryProgressDialog::setImagePath(CPFilePathContainer* pathContainer)
{
	_imageDirWidget->setPath(pathContainer->getDirWidget()->getPath());
	_filenameEdit->setText(pathContainer->getNameWidget()->text());
}