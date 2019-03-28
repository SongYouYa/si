#include "stdafx.h"
#include "PPageTabButton.h"

extern const int TAB_HEIGHT;
const int TAB_WIDTH = 90;

CPPageTabButton::CPPageTabButton(const BtnConfig& config, QWidget* parent)
	: CPTabButton(config, parent)
{
	setObjectName("main_tab_button");

	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setIconSize(QSize(35, 33));

	setFixedHeight(TAB_HEIGHT);
	setFixedWidth(TAB_WIDTH);
}
