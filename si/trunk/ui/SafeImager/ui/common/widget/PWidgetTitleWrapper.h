#pragma once
#include "uicommon/device/PTitleWrapperBase.h"

class CPWidgetTitleWrapper : public puic::CPTitleWrapperBase
{
public:
	CPWidgetTitleWrapper(QWidget* widget, QWidget* parent = nullptr);

	void setTitle(const QString& title);
	void addRightButton(QWidget *button);

private:
	QHBoxLayout* _topLayout;
	QLabel* _titleLabel;
};
