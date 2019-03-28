#include "stdafx.h"
#include "PWidgetTitleWrapper.h"


CPWidgetTitleWrapper::CPWidgetTitleWrapper(QWidget* widget, QWidget* parent)//添加标题
	: CPTitleWrapperBase(widget, parent)
{
	QVBoxLayout* layout = new QVBoxLayout();
	layout->setMargin(0);
	layout->setSpacing(0);

	QWidget* topWidget = new QWidget();
	topWidget->setObjectName("content_container");
	_topLayout = new QHBoxLayout();
	_topLayout->setContentsMargins(0, 0, 0, 6);
	_titleLabel = new QLabel();
	_titleLabel->setObjectName("title1_label");
	_topLayout->addWidget(_titleLabel);
	_topLayout->addStretch();
	topWidget->setLayout(_topLayout);

	layout->addWidget(topWidget);
	layout->addWidget(_contentWidget);
	setLayout(layout);
}

void CPWidgetTitleWrapper::setTitle(const QString& title)
{
	_titleLabel->setText(title);
}

void CPWidgetTitleWrapper::addRightButton(QWidget *button)//右边添加button
{
	_topLayout->addWidget(button);
}
