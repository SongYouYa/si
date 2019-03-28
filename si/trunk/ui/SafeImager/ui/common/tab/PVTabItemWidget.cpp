#include "stdafx.h"
#include "PVTabItemWidget.h"


const int COLLAPSE_LIMIT_WIDTH = 70;

CPVTabItemWidget::CPVTabItemWidget(const Config& config, QWidget* parent)
	: QFrame(parent), _config(config)
{
	setObjectName("vtab_item_widget");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(10);
	
	_leftSpacing = new QWidget();
	_leftSpacing->setFixedWidth(30);
	layout->addWidget(_leftSpacing);

	_iconLabel = new QLabel();
	_iconLabel->setObjectName("icon_label");
	if (!config.icon.isEmpty()) {
		_iconLabel->setPixmap(config.icon);
	}
	layout->addWidget(_iconLabel);

	_iconTextSpace = new QWidget();
	_iconTextSpace->setFixedWidth(12);
	layout->addWidget(_iconTextSpace);

	_textLabel = new QLabel();
	_textLabel->setText(config.text);
	layout->addWidget(_textLabel);

	layout->addStretch();
	setLayout(layout);
}

void CPVTabItemWidget::setCollapsed(bool collapsed)
{
	_leftSpacing->setVisible(!collapsed);
	_iconTextSpace->setVisible(!collapsed);
	_textLabel->setVisible(!collapsed);
	if (collapsed) {
		setToolTip(_config.text);
	}
	else {
		setToolTip("");
	}
}

void CPVTabItemWidget::resizeEvent(QResizeEvent * event)
{
	QSize size = event->size();
	//std::cout << "tab item width: " << size.width() << std::endl;
	if (size.width() < COLLAPSE_LIMIT_WIDTH) {
		setCollapsed(true);
	}
	else {
		setCollapsed(false);
	}
	QWidget::resizeEvent(event);
}
