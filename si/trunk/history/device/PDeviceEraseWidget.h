#pragma once
#include "uicommon/device/PDeviceLocalDecorator.h"


class CPDeviceEraseWidget : public QWidget
{
	Q_OBJECT
public:
	CPDeviceEraseWidget(QWidget* parent = nullptr);
	void initView(const pdevice::DeviceDisks& allDisks, 
		const pdevice::DeviceDisks& usbDisks);

protected:
	//qss对基类QWidget不起作用，需要重写此函数
	void paintEvent(QPaintEvent *e);

	private slots:
	void slotStartTask();
private:

	CPDeviceTreeWidget* _targetTree;
	CPDeviceLocalDecorator* _localDecorator;

	QRadioButton* _eraseSimpleBtn;
	QRadioButton* _eraseBmbBtn;
	QRadioButton* _eraseDodBtn;

	static CPRecord* s_eraseRecord;
};
