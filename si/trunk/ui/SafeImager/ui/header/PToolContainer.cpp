#include "stdafx.h"
#include "PToolContainer.h"


const QSize ToolButtonSize = QSize(23, 23);

CPToolContainer::CPToolContainer(QWidget* parent)
	: QFrame(parent)
{
	setObjectName("header_tool_container");
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(10);

	QToolButton* caseButton = new QToolButton();
	caseButton->setObjectName("case");
	caseButton->setFixedSize(ToolButtonSize);
	caseButton->setToolTip(tr("case info"));
	layout->addWidget(caseButton);

	QToolButton* cmdButton = new QToolButton();
	cmdButton->setObjectName("cmd");
	cmdButton->setFixedSize(ToolButtonSize);
	cmdButton->setToolTip(tr("cmd"));
#ifdef OS_PWINDOWS
	//只有windows在线需要可信命令行
	layout->addWidget(cmdButton);
#endif

	QToolButton* photoButton = new QToolButton();
	photoButton->setObjectName("photo");
	photoButton->setFixedSize(ToolButtonSize);
	photoButton->setToolTip(tr("photo"));
	layout->addWidget(photoButton);

	QToolButton* videoButton = new QToolButton();
	videoButton->setObjectName("video");
	videoButton->setFixedSize(ToolButtonSize);
	videoButton->setIconSize(ToolButtonSize);
	videoButton->setToolTip(tr("video"));
	layout->addWidget(videoButton);

	setLayout(layout);

	_toolsHandler = new CPToolsHandler(this);
	_toolsHandler->setCaseButton(caseButton);
	_toolsHandler->setCmdButton(cmdButton);
	_toolsHandler->setPhotoButton(photoButton);
	_toolsHandler->setVideoButton(videoButton);

}

void CPToolContainer::setMainWindow(QWidget* widget)
{
	_toolsHandler->setMainWindow(widget);
}