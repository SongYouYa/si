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

	//����Qt::QueuedConnection������Ϊ�����豸�仯���źţ����ܺ�ui�߳��ǲ�ͬ���߳�
	//���½�����Ҫ����ui�߳�
	connect(this, &CPDeviceViewHandler::qsignalDeviceChanged, this, [this](){
		updateDevices();
	}, Qt::QueuedConnection);
}

void CPDeviceViewHandler::updateDevices()
{
	using namespace pdevice;

	DeviceDisks allDisks;
	CPDeviceManager::get()->getAllDisks(allDisks);
	DeviceDisks allFixedDisks; //������CD
	CPDeviceManager::get()->getAllFixedDisks(allFixedDisks);
	DevicePartitions logicalPartitions;
    CPDeviceManager::get()->getLocalLogicalPartitions(logicalPartitions);

	DeviceDisks allNonsiDisks;
	DeviceDisks localDisks; //����â���ƶ��豸
	DeviceDisks usbDisks; //���ƶ��豸
	for (auto& disk : allDisks) {
		if (CPString::isEquali(disk.model, L"SafeImager") ||
			CPString::isEquali(disk.model, L"SafeImagerMac")) {
			continue;
		}
		allNonsiDisks.push_back(disk);
		//Ŀ�����ֻ����Ӳ�̣�CD��Զ�̴��̲�֧��
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
