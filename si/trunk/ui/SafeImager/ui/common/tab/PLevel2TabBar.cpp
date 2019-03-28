#include "stdafx.h"
#include "PLevel2TabBar.h"


CPVTab2ItemWidget::CPVTab2ItemWidget(const Config& config, QWidget* parent)
	: QWidget(parent),
	_config(config)
{
	setObjectName("vtab2_item_widget");

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setSpacing(10);
	layout->setMargin(5);

	layout->addStretch(1);

	_iconLabel = new QLabel();
	_iconLabel->setObjectName("icon_label");
	_iconLabel->setFixedSize(QSize(3, 14));
	_iconLabel->hide();
	_spaceLabel = new QLabel();
	_spaceLabel->setObjectName("space_label");
	_spaceLabel->setFixedSize(QSize(3, 14));
	layout->addWidget(_iconLabel);
	layout->addWidget(_spaceLabel);

	_textLabel = new QLabel();
	_textLabel->setText(config.text);
	layout->addWidget(_textLabel);

	layout->addStretch(3);

	setLayout(layout);
}

void CPVTab2ItemWidget::setChecked(bool checked)
{
	if (checked) {
		_iconLabel->show();
		_spaceLabel->hide();
	}
	else {
		_iconLabel->hide();
		_spaceLabel->show();
	}
}

void CPVTab2ItemWidget::setCount(int count)
{
	QString countText = " (" + QString::number(count) + ")";
	_textLabel->setText(_config.text + countText);
}


CPLevel2TabBar::CPLevel2TabBar(const QString& configPath, QWidget* parent)
	: CPVTabBar(configPath, parent)
{
	_container->setObjectName("vtabbar2_container");
	setMaximumWidth(180);
}

QWidget* CPLevel2TabBar::newItemWidget(const CPVTabItemWidget::Config& config)
{
	CPVTab2ItemWidget::Config config2;
	config2.text = config.text;
	return new CPVTab2ItemWidget(config2);
}

void CPLevel2TabBar::slotItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	CPVTab2ItemWidget* preWidget = qobject_cast<CPVTab2ItemWidget*>(
		_treeWidget->itemWidget(previous, 0));
	if (preWidget) {
		preWidget->setChecked(false);
	}
	CPVTab2ItemWidget* curWidget = qobject_cast<CPVTab2ItemWidget*>(
		_treeWidget->itemWidget(current, 0));
	if (curWidget) {
		curWidget->setChecked(true);
	}
	CPVTabBar::slotItemChanged(current, previous);
}