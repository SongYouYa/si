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
	//Ӧ���ļ���������
	void signalAppParsed();

private:
	void initLeftLayout() override;
	void getToolExpr(QString& toolExpr);
	void startFilte(bool force = false) override;
	//Ӧ���ļ�������Ҫ���⴦��һ��
	void onAppFilterClicked(bool checked);

	std::map<QPushButton*, QString> _tool_exprs;
	QPushButton* _appButton; //Ӧ���ļ����˰�ť

	std::vector<QWidget*> _exAppwidgets;
};