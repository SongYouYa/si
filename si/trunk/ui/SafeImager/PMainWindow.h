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
	//�ڴ������н�����еĽ�����Ҫ��ʼ���������ﴥ��
	void initContents();

	CPAppView* _appView;
	CPDeviceCopyView* _copyView;
};
