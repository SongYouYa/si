#pragma once
#include "uicommon/button/PToolButton.h"

class CPOperationButton : public CPToolButton
{
	Q_OBJECT
public:
	CPOperationButton(const BtnConfig& config, QWidget* parent = nullptr);

	//ͨ�õġ�ȡ֤����ť
	static QPushButton* newForensicButton(QWidget* parent = nullptr);
};