#pragma once
#include "ui/common/filter/PFilterBarWidget.h"

class CPFsFilterBar : public CPFilterBarWidget
{
	Q_OBJECT
public:
	CPFsFilterBar(QWidget* parent = nullptr);

	private slots:
	void slotAppParsed();

signals:
	//应用文件分析结束
	void signalAppParsed();

private:
	void initLeftLayout() override;
	void getToolExpr(QString& toolExpr);
	void startFilte(bool force = false) override;
	//应用文件过滤需要特殊处理一下
	void onAppFilterClicked(bool checked);

	std::map<QPushButton*, QString> _tool_exprs;
	QPushButton* _appButton; //应用文件过滤按钮

	std::vector<QWidget*> _exAppwidgets;
};