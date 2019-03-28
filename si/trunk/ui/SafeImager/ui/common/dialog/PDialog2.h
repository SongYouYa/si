#pragma once
#include "uicommon/dialog/PDialog.h"

class CPDialog2 : public puic::CPDialog
{
	Q_OBJECT
public:
	CPDialog2(QWidget* parent = nullptr);

protected:
	void setupButtons() override;
};