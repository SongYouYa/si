#pragma once
#include "./PTabBarWidget.h"
#include "./PToolContainer.h"

class CPHeaderWidget : public QFrame
{
	Q_OBJECT
public:
	CPHeaderWidget(QWidget* parent = nullptr);

	CPTabBar* tabBar() {
		return _tabBar;
	}

	void setMainWindow(QWidget* widget);

private:
	CPTabBar* _tabBar;
	CPToolContainer* _toolContainer;
	CPMinmaxWidget* _minmaxWidget;
};
