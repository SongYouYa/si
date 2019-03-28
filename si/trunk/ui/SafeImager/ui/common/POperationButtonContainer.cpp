#include "stdafx.h"
#include "POperationButtonContainer.h"


CPOperationButtonContainer::CPOperationButtonContainer(QWidget* parent)
	: QFrame(parent)
{
	setObjectName("operation_button_container");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setContentsMargins(10, 10, 15, 10);

	_leftLayout = new QHBoxLayout();
	layout->addLayout(_leftLayout);

	layout->addStretch(1);

	_rightLayout = new QHBoxLayout();
	layout->addLayout(_rightLayout);

	setLayout(layout);
}

void CPOperationButtonContainer::addLeftWidget(QWidget* widget)
{
	_leftLayout->addWidget(widget);
}

void CPOperationButtonContainer::addRightWidget(QWidget* widget)
{
	_rightLayout->addWidget(widget);
}