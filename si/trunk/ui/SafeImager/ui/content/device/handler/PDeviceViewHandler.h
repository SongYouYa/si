#pragma once
#include "ui/content/device/PDeviceCloneWidget.h"
#include "ui/content/device/PDeviceImageWidget.h"
#include "ui/content/device/PMemoryImageWidget.h"

class CPDeviceViewHandler : public QObject
{
	Q_OBJECT
public:
	
    static CPDeviceViewHandler* get(CPDeviceCloneWidget* cloneWidget = nullptr,
        CPDeviceImageWidget* imageWidget = nullptr);

	void updateDevices();

signals:
	void qsignalDeviceChanged();

private:
    CPDeviceViewHandler(CPDeviceCloneWidget* cloneWidget,
        CPDeviceImageWidget* imageWidget);
	void slotDeviceChanged();

	CPDeviceCloneWidget* _cloneWidget;
	CPDeviceImageWidget* _imageWidget;

	static CPDeviceViewHandler* _instance;
};
