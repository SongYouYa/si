#include "stdafx.h"
#include "PMemoryImageDialog.h"
#include "PMemoryImageWidget.h"
#include "ui/common/POperationButtonContainer.h"


CPMemoryImageDialog::CPMemoryImageDialog(QWidget* parent)
	: CPDialog2(parent)
{
	setWindowTitle(tr("memory dump"));
	enableResizer(true);
#ifdef OS_PAPPLE
    setFixedWidth(500);
#endif
	setOkButtonText(tr("forensic"));
	setCancelButtonText(tr("skip"));

	QWidget* container = new QWidget();
	QVBoxLayout* contentLayout = new QVBoxLayout();
	_imageWidget = new CPMemoryImageWidget();
	_imageWidget->setBeforeCase(true);
	_imageWidget->getButtonContainer()->setVisible(false);
	_imageWidget->initWidget();
	setContentWidget(_imageWidget);
	setupButtons();

	connect(_imageWidget, &CPMemoryImageWidget::signalStarted, this, [this](){
		this->done(CPDialog::Accepted);
		onImageStart();
	});
	connect(_imageWidget, &CPMemoryImageWidget::signalCanceled, this, [this](){
		this->done(CPDialog::Rejected);
	});

	_imageWidget->addTaskCreateCallback([this](CPTask* task){
		_progressDialog.setTask(task);
	});

	std::wstring imageDir = CPCase::get()->caseRootDir();
	_imageWidget->getImagePath()->getDirWidget()->setDefaultDir(imageDir);
}

void CPMemoryImageDialog::onOkClicked()
{
	_imageWidget->slotStartButtonClicked();
}

void CPMemoryImageDialog::closeEvent(QCloseEvent* e)
{
	int ret = CPMessageBox::question(tr("cancel memory image?"));
	if (ret == CPDialog::Accepted) {
		CPDialog::closeEvent(e);
	}
	else {
		e->ignore();
	}
}

void CPMemoryImageDialog::onImageStart()
{
	CPFilePathContainer* optionPath = _imageWidget->getImagePath();
	_progressDialog.setImagePath(optionPath);
	_progressDialog.exec();
}
