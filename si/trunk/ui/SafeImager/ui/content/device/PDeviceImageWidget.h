#pragma once
#include "./PImageWidgetBase.h"

class CPDeviceImageWidget : public CPImageWidgetBase
{
	Q_OBJECT
public:
	CPDeviceImageWidget(QWidget* parent = nullptr);
	void initWidget();
	void setDevices(const pdevice::DeviceDisks& disks,
		const pdevice::DeviceDisks& localDisks,
		const pdevice::DevicePartitions& logicalPartitions);

	protected slots:
	void slotStartButtonClicked();

protected:
    void getDeviceParam(const libdt::OneDevice&oneDevice, libdt::TDeviceParam& devParam) override;
#ifdef OS_PAPPLE
    bool getVolumePassword(devtask::DeviceImageParam& param);
#endif

private:
	CPDeviceUsbDecorator* _treeDecorator;
};
