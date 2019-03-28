#include "stdafx.h"
#include "PTabBarWidget.h"
#include "PPageTabButton.h"

extern const int TAB_HEIGHT = 70;

CPTabBarWidget::CPTabBarWidget(const QString& configPath, QWidget* parent)
	: CPTabBar(configPath, parent),
	_layout(nullptr)
{
	setFixedHeight(TAB_HEIGHT);

	_layout = new QHBoxLayout();
	_layout->setSpacing(0);
	_layout->setContentsMargins(0, 0, 0, 0);
	_container->setObjectName("header_tabbar");
	_container->setLayout(_layout);
}

QWidget* CPTabBarWidget::addTab(const QString& tabName, const QString& displayName)
{
	QJsonObject tabJson = _configs["tabs"].toObject()[tabName].toObject();
	CPTabButton::BtnConfig btnConfig;
	btnConfig.text = displayName;
	btnConfig.icon_normal = tabJson["icon_normal"].toString();
	btnConfig.icon_hover = tabJson["icon_active"].toString();
	CPPageTabButton* tabButton = new CPPageTabButton(btnConfig);

	_layout->addWidget(tabButton);

	_tabButtons[tabName] = tabButton;

	connect(tabButton, &QToolButton::clicked, this, [this, tabName]() {
		emit signalTabChanged(tabName);
	});

	return tabButton;
}

void CPTabBarWidget::endArea()
{
	_layout->addStretch();
}

void CPTabBarWidget::setCurTab(const QString& tabName)
{
	auto it = _tabButtons.find(tabName);
	if (it != _tabButtons.end()) {
		//更新选中按钮
		auto it_b = _tabButtons.begin();
		for (; it_b != _tabButtons.end(); ++it_b) {
			if (it_b != it) {
				CPTabButton* tabButton = static_cast<CPTabButton*>(it_b->second);
				tabButton->setChecked(false);
			}
		}
		static_cast<CPTabButton*>(it->second)->setChecked(true);
	}
	else {
		assert(false);
	}
}