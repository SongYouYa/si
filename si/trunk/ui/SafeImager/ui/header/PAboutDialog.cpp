#include "stdafx.h"
#include "PAboutDialog.h"
#include "uicommon/dialog/PMinmaxWidget.h"
#include "uicommon/dialog/handler/PWindowMover.h"

CPAboutDialog::CPAboutDialog(QWidget* parent)
	: QDialog(parent)
{
	setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	resize(QSize(509, 324));

	QVBoxLayout* mlayout = new QVBoxLayout();
	mlayout->setMargin(0);

	QWidget* container = new QWidget();
	container->setObjectName("about_dialog");

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(15);

	QHBoxLayout* headerLayout = new QHBoxLayout();
	headerLayout->addStretch();
	auto closeButton = new CPMinmaxWidget(this);
	closeButton->setTargetWidget(this);
	headerLayout->addWidget(closeButton);

	layout->addLayout(headerLayout);
	layout->addStretch(2);

	QHBoxLayout* versionLayout = new QHBoxLayout();
	versionLayout->setSpacing(5);
	versionLayout->addStretch();
	QLabel* versionTitle = new QLabel(tr("version:"));
	QLabel* versionValue = new QLabel();
	versionValue->setText(QString::fromStdWString(CPAppContext::get()->version()));
	versionLayout->addWidget(versionTitle);
	versionLayout->addWidget(versionValue);
	versionLayout->addStretch();

	layout->addLayout(versionLayout);

	layout->addStretch(3);

	container->setLayout(layout);
	mlayout->addWidget(container);
	setLayout(mlayout);

	CPWindowMover* mover = new CPWindowMover(this);
	mover->setTarget(this, this);
	mover->enableDoubleClick(false);
}