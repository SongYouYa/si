#pragma once
#include "uicommon/progress/PLoadingWidget.h"
#include "uicommon/record/PAppTreelistWidget.h"

class CPAppView : public QWidget
{
	Q_OBJECT
public:
	CPAppView(QWidget* parent = nullptr);

	void initView();

	protected slots:
	void startAppTask();
	//�������ͱ仯ʱ������������Ӧ�仯
	void slotCatagoryChanged();

private:
	//��ʼ��appѡ����
	void startInitTask();

	CPLoadingWidget* _loadingWidget;
	CPAppTreelistWidget* _appTreelist;
	CPRecord* _appRawRootRecord; //δ�����ļ�ϵͳǰ����õ�record���ڵ�
	bool _appInited; //�Ѿ������ļ�ϵͳ����ʼ��
	QPushButton* _startButton;
};