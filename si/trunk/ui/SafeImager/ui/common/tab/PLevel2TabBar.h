#pragma once
#include "./PVTabBar.h"
#include "./PVTabItemWidget.h"

class CPVTab2ItemWidget : public QWidget
{
	Q_OBJECT
public:
	struct Config
	{
		QString text;
	};
	CPVTab2ItemWidget(const Config& config, QWidget* parent = nullptr);

	void setChecked(bool checked);
	void setCount(int count);

private:
	Config _config;
	QLabel* _iconLabel;
	QLabel* _spaceLabel;
	QLabel* _textLabel;
};

class CPLevel2TabBar : public CPVTabBar
{
public:
	CPLevel2TabBar(const QString& configPath, QWidget* parent = nullptr);

protected:
	QWidget* newItemWidget(const CPVTabItemWidget::Config& config) override;

	void slotItemChanged(QTreeWidgetItem*, QTreeWidgetItem*) override;
};