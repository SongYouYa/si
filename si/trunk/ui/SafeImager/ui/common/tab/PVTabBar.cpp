#include "stdafx.h"
#include "PVTabBar.h"


CPVTabBar::CPVTabBar(const QString& configPath, QWidget* parent)
	: CPTabBar(configPath, parent)
{
	_container->setObjectName("vtabbar_container");

	QVBoxLayout* layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setContentsMargins(0, 0, 0, 0);
	_treeWidget = new QTreeWidget();
	_treeWidget->setHeaderHidden(true);
	_treeWidget->setIndentation(0); //缩进
	_treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	_treeWidget->setColumnCount(1);
	//_treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	layout->addWidget(_treeWidget);

	_container->setLayout(layout);

	connect(_treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), 
		this, SLOT(slotItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
}

QWidget* CPVTabBar::newItemWidget(const CPVTabItemWidget::Config& config)
{
	CPVTabItemWidget* tabItem = new CPVTabItemWidget(config);
	return tabItem;
}

QWidget* CPVTabBar::addTab(const QString& tabName, const QString& displayName)
{
	QJsonObject tabsJson = _configs["tabs"].toObject();
	QJsonObject tabJson = tabsJson[tabName].toObject();
	CPVTabItemWidget::Config btnConfig;
	btnConfig.text = displayName;
	btnConfig.icon = tabJson["icon"].toString();
	QWidget* tabItem = newItemWidget(btnConfig);

	QTreeWidgetItem* treeItem = new QTreeWidgetItem();
	_treeWidget->addTopLevelItem(treeItem);
	_treeWidget->setItemWidget(treeItem, 0, tabItem);

	_tabItems[tabName] = treeItem;
	return tabItem;
}

void CPVTabBar::endArea()
{
	
}

void CPVTabBar::setCurTab(const QString& tabName)
{
	auto it = _tabItems.find(tabName);
	if (it == _tabItems.end()) {
		return;
	}
	_treeWidget->setCurrentItem(it->second);
}

void CPVTabBar::slotItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	for (auto& it : _tabItems) {
		if (it.second == current) {
			emit signalTabChanged(it.first);
			break;
		}
	}
}

void CPVTabBar::setMinWidth(int width)
{
	//这句是必须的，否则能够缩小的最大宽度为100px
	_treeWidget->header()->resizeSection(0, width);
}