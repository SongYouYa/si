#pragma once
#include "uicommon/device/PDeviceTreeWidget.h"
#include "uicommon/device/PDeviceUsbDecorator.h"
#include "uicommon/device/PDeviceLocalDecorator.h"

#include "./material/PHashOptionWidget2.h"

class CPDeviceCloneWidget : public QWidget
{
	Q_OBJECT
public:
	CPDeviceCloneWidget(QWidget* parent = nullptr);
	void setDevices(const pdevice::DeviceDisks& allDisks, 
		const pdevice::DeviceDisks& localDisks,
		const pdevice::DeviceDisks& usbDisks);

	private slots:
	void slotStartTask();
private:

	CPDeviceTreeWidget* _sourceTree;
	CPDeviceUsbDecorator* _usbDecorator;

	CPDeviceTreeWidget* _targetTree;
	CPDeviceLocalDecorator* _localDecorator;
	QCheckBox* _eraseTailBox;

	CPHashOptionWidget2* _hashWidget;
};
