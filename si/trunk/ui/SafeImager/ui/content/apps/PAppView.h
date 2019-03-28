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
	//案件类型变化时，分析打勾项相应变化
	void slotCatagoryChanged();

private:
	//初始化app选项树
	void startInitTask();

	CPLoadingWidget* _loadingWidget;
	CPAppTreelistWidget* _appTreelist;
	CPRecord* _appRawRootRecord; //未加载文件系统前，获得的record根节点
	bool _appInited; //已经加载文件系统并初始化
	QPushButton* _startButton;
};