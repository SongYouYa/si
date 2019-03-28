#include "stdafx.h"
#include "PDeviceImageWidget.h"
#include "ui/common/widget/PWidgetTitleWrapper.h"
#include "ui/common/POperationButtonContainer.h"
#include "ui/common/POperationButton.h"

#include "uicommon/device/PDeviceUiUtils.h"
#include "libdevtask/PDeviceTaskUtils.h"

#include "handler/PDeviceTaskHandler.h"

#include "ui/content/device/material/PVolumePasswordDialog.h"

using namespace devtask;
using namespace pdevice;
using namespace libdt;

extern CPTabManager* g_mainTab;

CPDeviceImageWidget::CPDeviceImageWidget(QWidget* parent)
	:CPImageWidgetBase(PDF_USB | PDF_RADIO | PDF_PARTITION | PDF_CUR_SELECT, parent)
{
	_treeDecorator = new CPDeviceUsbDecorator(_sourceWrapper, this);
}

void CPDeviceImageWidget::initWidget()
{
	CPImageWidgetBase::initCallback();
}

void CPDeviceImageWidget::setDevices(const pdevice::DeviceDisks& disks,
	const pdevice::DeviceDisks& localDisks,
	const pdevice::DevicePartitions& logicalPartitions)
{
	_treeDecorator->initView(disks, localDisks, &logicalPartitions);
}

void CPDeviceImageWidget::getDeviceParam(const libdt::OneDevice&oneDevice, libdt::TDeviceParam& devParam)
{
    libdt::CPDeviceTaskUtils::getDeviceParam(oneDevice, devParam);
}

#ifdef OS_PAPPLE
bool CPDeviceImageWidget::getVolumePassword(devtask::DeviceImageParam &param)
{
    //mac下如果加密卷未unlock，需要获取其密码
    using namespace pdevice;
    if (param.sourceDevice.isPartition) {
        DevicePartition sourcePartition;
        bool ret = CPDeviceManager::get()->getPartitionByName(
                    param.sourceDevice.deviceName, sourcePartition);
        assert(ret);
        if (sourcePartition.locked) {
            CPVolumePasswordDialog passDialog;
            int nret = passDialog.exec();
            if (nret != CPDialog::Accepted) {
                return false;
            }
            param.volumePassword = passDialog.password().toStdString();
        }
    }
    return true;
}
#endif

void CPDeviceImageWidget::slotStartButtonClicked()
{
	DeviceImageParam imageParam;
	//检测选中磁盘数量和读写
	libdt::OneDevices sourceDevices;
	if (!checkSelect(sourceDevices)) {
		return;
	}
	if (!CPDeviceUiUtils::checkReadable(sourceDevices)) {
		return;
	}
	assert(sourceDevices.size() == 1);
	imageParam.sourceDevice = sourceDevices.at(0);

	libdt::TDeviceParam devParam;
	libdt::CPDeviceTaskUtils::getDeviceParam(imageParam.sourceDevice, devParam);

    _imageOptionWidget->getOptions(imageParam.formatOptions);
	_hashWidget->getHashOptions(imageParam.hashTypes);

	if (!checkTargetPath(imageParam.formatOptions.imageType,
		devParam.deviceSize, imageParam.formatOptions.splitSize)) {
		return;
	}
	imageParam.imagePath = _filePathContainer->getPath();
#ifdef OS_PAPPLE
    if (!getVolumePassword(imageParam)) {
        return;
    }
#endif
	if (!checkDestExist(imageParam.imagePath)) {
		return;
	}

    if (!CPCaseHandler::checkCaseCreated()) {
        return;
    }

	CPTask* task = CPDeviceTaskHandler::createImageTask(imageParam);
    if (!task) {
        return;
    }

#if 0
    g_mainTab->navToTab(PTAB_TASK);//切换页面
#endif
    _sourceTree->selectAll(false);

	CPTaskController::get()->doTask(task);
}
