#pragma once
#include "uicommon/button/PToolButton.h"

class CPOperationButton : public CPToolButton
{
	Q_OBJECT
public:
	CPOperationButton(const BtnConfig& config, QWidget* parent = nullptr);

	//通用的“取证”按钮
	static QPushButton* newForensicButton(QWidget* parent = nullptr);
};