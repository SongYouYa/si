#include "stdafx.h"
#include "POperationButton.h"


CPOperationButton::CPOperationButton(const BtnConfig& config, QWidget* parent)
	:CPToolButton(config, parent)
{

}

QPushButton* CPOperationButton::newForensicButton(QWidget* parent)
{
	QPushButton* button = new QPushButton(tr("forensic"), parent);
	button->setObjectName("forensic_button");
	button->setFixedSize(103, 33);
	return button;
}