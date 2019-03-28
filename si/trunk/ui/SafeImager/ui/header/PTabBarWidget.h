#pragma once
#include "uicommon/tab/PTabBar.h"

class CPTabBarWidget : public puic::CPTabBar
{
	Q_OBJECT
public:
	CPTabBarWidget(const QString& configPath, QWidget* parent = nullptr);

protected:
	QWidget* addTab(const QString& tabName, const QString& displayName) override;
	void endArea();
	void setCurTab(const QString& tabName);

private:
	QHBoxLayout* _layout;

	std::map<QString, QWidget*> _tabButtons;
};