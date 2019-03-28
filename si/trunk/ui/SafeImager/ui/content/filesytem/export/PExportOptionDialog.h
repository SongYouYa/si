#pragma once
#include "ui/common/widget/PDirSelectWidget.h"

class CPExportOptionDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPExportOptionDialog(QWidget* parent = nullptr);
	void getExportParam(devtask::FileExportParam& exportParam);

private:
	CPDirSelectWidget* _selectWidget;
};