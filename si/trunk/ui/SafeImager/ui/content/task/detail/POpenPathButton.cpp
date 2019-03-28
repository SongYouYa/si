#include "stdafx.h"
#include "POpenPathButton.h"

#include "qtutility/PPathUtil.h"


CPOpenPathButton::CPOpenPathButton(const std::wstring& openPath, QWidget* parent)
	: QPushButton(parent)
{
	setObjectName("task_detail_button");
	setToolTip(tr("open path"));

	_imagePath = openPath;
	//定义按钮展示导出路径界面
	connect(this, SIGNAL(clicked()), this, SLOT(showPathWidget()));
}

void CPOpenPathButton::showPathWidget()
{
	qtutil::CPPathUtil::openExplorer(_imagePath);
}
