#include "stdafx.h"
#include "PNavigationWidget.h"
#include "PTabItemWidget.h"

const int MIN_WIDTH = 35;
const int MAX_WIDTH = 200;


CPNavigationWidget::CPNavigationWidget(QWidget* parent)
	: QWidget(parent)
{
	setObjectName("main_nav");

	setMinimumWidth(MIN_WIDTH);

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);

	CPTabItemWidget::Config itemConfig;
    itemConfig.text = "SafeImager";
	auto* headerLogoWidget = new CPTabHeaderItemWidget(itemConfig);
	layout->addWidget(headerLogoWidget);

	_tabBar = new CPVTabBar(":/styles/config/maintab.json", this);//Tab
	_tabBar->setObjectName("main_tab");
	_tabBar->setMinWidth(MIN_WIDTH);
	layout->addWidget(_tabBar);

	setLayout(layout);

	connect(headerLogoWidget, &CPTabHeaderItemWidget::signalToggleCollapse, this, [this](){
		emit signalToggleCollapse();
	});
}

int CPNavigationWidget::minWidth()
{
	return MIN_WIDTH;
}

int CPNavigationWidget::maxWidth()
{
	return MAX_WIDTH;
}
