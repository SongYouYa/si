#include "stdafx.h"
#include "POpenPathButton.h"

#include "qtutility/PPathUtil.h"


CPOpenPathButton::CPOpenPathButton(const std::wstring& openPath, QWidget* parent)
	: QPushButton(parent)
{
	setObjectName("task_detail_button");
	setToolTip(tr("open path"));

	_imagePath = openPath;
	//���尴ťչʾ����·������
	connect(this, SIGNAL(clicked()), this, SLOT(showPathWidget()));
}

void CPOpenPathButton::showPathWidget()
{
	qtutil::CPPathUtil::openExplorer(_imagePath);
}
