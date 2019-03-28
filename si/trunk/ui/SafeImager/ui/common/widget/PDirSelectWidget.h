#pragma once
#include "uicommon/widget/PPathSelectWidget.h"

class CPDirSelectWidget : public puic::CPPathSelectWidget
{
	Q_OBJECT
public:
	CPDirSelectWidget(QWidget* parent = nullptr);
};


class CPFileNameEdit : public QLineEdit
{
public:
	CPFileNameEdit(QWidget *parent = nullptr);
};


class CPFilePathContainer : public QWidget
{
	Q_OBJECT
public:
	CPFilePathContainer(QWidget* parent = nullptr);

	CPDirSelectWidget* getDirWidget() {
		return _imageDirWidget;
	}
	CPFileNameEdit* getNameWidget() {
		return _filenameEdit;
	}

	std::wstring getPath();

private:
	CPDirSelectWidget* _imageDirWidget;
	CPFileNameEdit* _filenameEdit;
};