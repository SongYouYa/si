#pragma once
#include "uicommon/button/PTabButton.h"

class CPPageTabButton : public puic::CPTabButton
{
	Q_OBJECT
public:
	CPPageTabButton(const BtnConfig& config, QWidget* parent = nullptr);
};
