#pragma once
#include "./ui/tab/PVTabBar.h"

class CPNavigationWidget : public QWidget
{
	Q_OBJECT
public:
	CPNavigationWidget(QWidget* parent = nullptr);

	int minWidth();
	int maxWidth();

	CPTabBar* tabBar() {
		return _tabBar;
	}

signals:
	void signalToggleCollapse();

private:
	CPVTabBar* _tabBar;
};
