#include "stdafx.h"
#include "PHeaderWidget.h"

#include "PLogoWidget.h"
#include "PDropDownWidget.h"
#include "uicommon/dialog/handler/PWindowMover.h"


CPHeaderWidget::CPHeaderWidget(QWidget* parent)
	: QFrame(parent)
{
	setObjectName("header_widget");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	CPLogoWidget* titleWidget = new CPLogoWidget();
	layout->addWidget(titleWidget);

	layout->addStretch(1);

	_tabBar = new CPTabBarWidget(":/styles/config/maintab.json");
	layout->addWidget(_tabBar, 1);
	layout->addStretch(1);

	//split line
	QWidget* splitLine = new QWidget(this);
	splitLine->setObjectName("vsplit_line");
	splitLine->setFixedSize(QSize(1, 40));
	layout->addWidget(splitLine);

	layout->addSpacing(10);

	_toolContainer = new CPToolContainer();
	layout->addWidget(_toolContainer, 0, Qt::AlignBottom);

	layout->addStretch(5);

	QHBoxLayout* rightLayout = new QHBoxLayout();
	rightLayout->setContentsMargins(5, 5, 5, 5);
	rightLayout->setSpacing(10);
	CPDropDownWidget* dropdown = new CPDropDownWidget();
	rightLayout->addWidget(dropdown, 0, Qt::AlignTop);

	_minmaxWidget = new CPMinmaxWidget(nullptr, WITH_BTN_MINIMUM | WITH_BTN_MAXIMUM);
	rightLayout->addWidget(_minmaxWidget, 0, Qt::AlignTop);

	layout->addLayout(rightLayout);

	setLayout(layout);
}

void CPHeaderWidget::setMainWindow(QWidget* widget)
{
	_toolContainer->setMainWindow(widget);
	_minmaxWidget->setTargetWidget(widget);
	CPWindowMover* mover = new CPWindowMover(this);
	mover->setTarget(this, widget);
}
