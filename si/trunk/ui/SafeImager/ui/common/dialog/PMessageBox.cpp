#include "stdafx.h"
#include "PMessageBox.h"


CPMessageBox::CPMessageBox(QWidget* parent)
	: CPDialog2(parent),
	_iconLabel(new QLabel(this)),
	_msgLabel(new QLabel(this))
{
	resize(QSize(350, 250));

	QWidget* container = new QWidget(this);
	container->setObjectName("content_container");
	container->setMinimumHeight(100);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->setSpacing(0);
	mainLayout->setMargin(0);
	mainLayout->addStretch(1);

	QHBoxLayout* contentLayout = new QHBoxLayout();
	contentLayout->setContentsMargins(20, 10, 20, 10);
	contentLayout->setSpacing(15);

	_iconLabel->setFixedSize(26, 26);
	contentLayout->addWidget(_iconLabel, 0/*, Qt::AlignTop*/);

	_msgLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
	_msgLabel->setWordWrap(true);
	contentLayout->addWidget(_msgLabel, 1/*, Qt::AlignTop*/);

	mainLayout->addLayout(contentLayout, 2);
	mainLayout->addStretch(3);
	container->setLayout(mainLayout);

	setContentWidget(container);
}

void CPMessageBox::setMsg(const QString& msg)
{
	_msgLabel->setText(msg);
}


int CPMessageBox::info(const QString& msg, QWidget* parent)
{
	CPMessageBox msgBox(parent);
	msgBox.setWindowTitle(tr("tip"));
	msgBox.setupButtons();
	msgBox._iconLabel->setPixmap(QPixmap(":/styles/icons/dialog/info.png"));
	msgBox.setCancelHidden(true);
	msgBox.setMsg(msg);
	return msgBox.exec();
}

int CPMessageBox::question(const QString& msg, QWidget* parent)
{
	CPMessageBox msgBox(parent);
	msgBox.setWindowTitle(tr("tip"));
	msgBox.setupButtons();
	msgBox._iconLabel->setPixmap(QPixmap(":/styles/icons/dialog/info.png"));
	msgBox.setMsg(msg);
	return msgBox.exec();
}

int CPMessageBox::questionEx(QWidget* parent, const QString& msg,
	const QString& okText, const QString& thirdText)
{
	CPMessageBox msgBox(parent);
	msgBox.setWindowTitle(tr("tip"));
	msgBox.setupButtons();
	msgBox._iconLabel->setPixmap(QPixmap(":/styles/icons/dialog/info.png"));
	msgBox.setMsg(msg);
	msgBox.setThirdHidden(false);
	msgBox.setThirdButtonText(thirdText);
	msgBox.setOkButtonText(okText);
	return  msgBox.exec();
}

int CPMessageBox::warning(const QString& msg, QWidget* parent)
{
	CPMessageBox msgBox(parent);
	msgBox.setWindowTitle(tr("tip"));
	msgBox.setupButtons();
	msgBox._iconLabel->setPixmap(QPixmap(":/styles/icons/dialog/info.png"));
	msgBox.setCancelHidden(true);
	msgBox.setMsg(msg);
	return msgBox.exec();
}

int CPMessageBox::error(const QString& msg, QWidget* parent)
{
	CPMessageBox msgBox(parent);
	msgBox.setWindowTitle(tr("tip"));
	msgBox.setupButtons();
	msgBox._iconLabel->setPixmap(QPixmap(":/styles/icons/dialog/info.png"));
	msgBox.setCancelHidden(true);
	msgBox.setMsg(msg);
	return msgBox.exec();
}