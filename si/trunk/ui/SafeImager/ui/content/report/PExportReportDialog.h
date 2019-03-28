#pragma once
#include "ui/common/widget/PDirSelectWidget.h"

class CPExportReportDialog : public CPDialog2
{
	Q_OBJECT
public:
	CPExportReportDialog(QWidget* parent = nullptr);

	void setDir(const std::wstring& dir);
	std::wstring getDir();

private:
	CPDirSelectWidget* _selectWidget;
};