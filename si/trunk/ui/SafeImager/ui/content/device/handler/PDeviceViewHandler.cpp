#include "stdafx.h"
#include "PDeviceViewHandler.h"


CPDeviceViewHandler* CPDeviceViewHandler::_instance = nullptr;

CPDeviceViewHandler* CPDeviceViewHandler::get(CPDeviceCloneWidget* cloneWidget,
	CPDeviceImageWidget* imageWidget)
{
	if (!_instance) {
		_instance = new CPDeviceViewHandler(cloneWidget, imageWidget);
	}
	return _instance;
}

CPDeviceViewHandler::CPDeviceViewHandler(CPDeviceCloneWidget* cloneWidget,
	CPDeviceImageWidget* imageWidget)
    : _cloneWidget(cloneWidget), _imageWidget(imageWidget)
{
	pdevice::CPDeviceManager::get()->connectDeviceChangeSignal(
		boost::bind(&CPDeviceViewHandler::slotDeviceChanged, this));

	//采用Qt::QueuedConnection，是因为发送设备变化的信号，可能和ui线程是不同的线程
	//更新界面需要采用ui线程
	connect(this, &CPDeviceViewHandler::qsignalDeviceChanged, this, [this](){
		updateDevices();
	}, Qt::QueuedConnection);
}

void CPDeviceViewHandler::updateDevices()
{
	using namespace pdevice;

	DeviceDisks allDisks;
	CPDeviceManager::get()->getAllDisks(allDisks);
	DeviceDisks allFixedDisks; //不包含CD
	CPDeviceManager::get()->getAllFixedDisks(allFixedDisks);
	DevicePartitions logicalPartitions;
    CPDeviceManager::get()->getLocalLogicalPartitions(logicalPartitions);

	DeviceDisks allNonsiDisks;
	DeviceDisks localDisks; //非五芒星移动设备
	DeviceDisks usbDisks; //非移动设备
	for (auto& disk : allDisks) {
		if (CPString::isEquali(disk.model, L"SafeImager") ||
			CPString::isEquali(disk.model, L"SafeImagerMac")) {
			continue;
		}
		allNonsiDisks.push_back(disk);
		//目标磁盘只能是硬盘，CD或远程磁盘不支持
		if (disk.removable && disk.mediaType == PMEDIA_TYPE_FIXED) {
			usbDisks.push_back(disk);
		}
		else {
			localDisks.push_back(disk);
		}
	}

	if (_cloneWidget) {
		_cloneWidget->setDevices(allNonsiDisks, localDisks, usbDisks);
	}
	if (_imageWidget) {
		_imageWidget->setDevices(allNonsiDisks, localDisks, logicalPartitions);
	}
}

void CPDeviceViewHandler::slotDeviceChanged()
{
	emit qsignalDeviceChanged();
}
