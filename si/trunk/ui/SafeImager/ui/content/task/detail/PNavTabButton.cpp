#include "stdafx.h"
#include "PNavTabButton.h"

extern CPTabManager* g_mainTab;

CPNavTabButton::CPNavTabButton(const QString& tabName, QWidget* parent)
	: QPushButton(parent)
{
	setObjectName("task_detail_button");
	setToolTip(tr("return tab"));

	_tabName = tabName;
	//定义按钮展示导出路径界面
	connect(this, &QPushButton::clicked, this, [this](){
		g_mainTab->navToTab(_tabName);
	});
}
