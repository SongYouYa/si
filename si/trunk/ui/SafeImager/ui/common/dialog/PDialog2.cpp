#include "stdafx.h"
#include "PDialog2.h"


CPDialog2::CPDialog2(QWidget* parent)
	: CPDialog(parent)
{

}

void CPDialog2::setupButtons()
{
	_buttonLayout->setSpacing(10);
	_buttonLayout->setContentsMargins(10, 10, 20, 10);
	_buttonLayout->addStretch();
	_buttonLayout->addWidget(_thirdButton);
    _buttonLayout->addSpacing(10);
	_buttonLayout->addWidget(_cancelButton);
    _buttonLayout->addSpacing(10);
	_buttonLayout->addWidget(_okButton);
	_okButton->setObjectName("dialog_ok_button");
}
