#pragma once

class CPAppView;
class CPDeviceCopyView;
class CPMainWindow : public puic::CPWindow
{
    Q_OBJECT
public:
    CPMainWindow(QWidget *parent = Q_NULLPTR);

protected:
	void closeEvent(QCloseEvent * event) override;

private:
	void createNavs();
	void createTabConents();
	//在创建所有界面后，有的界面需要初始化，在这里触发
	void initContents();

	CPAppView* _appView;
	CPDeviceCopyView* _copyView;
};
