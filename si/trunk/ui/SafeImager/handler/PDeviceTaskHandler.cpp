#include "stdafx.h"
#include "./PDeviceTaskHandler.h"

#include "ui/common/PFilePathChecker.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "libdevtask/PTaskError.h"

using namespace libdt;

extern CPRecord* g_cloneFolder;
extern CPRecord* g_imageFolder;

CPDeviceTaskHandler::CPDeviceTaskHandler()
{

}

CPTask* CPDeviceTaskHandler::createCloneTask(const devtask::DeviceCloneParam& param,
	const std::wstring& taskId)
{
	TaskParamPtr taskParam = std::make_shared<boost::any>(param);
	CPTask* task = CPTaskController::get()->createTask(L"task.deviceclone", taskParam, true, taskId);

	//关联警告
	task->addUiWarningCallback([task](int warning){
		if (TASK_WARNING_DEV_LOST == warning) {
			QString qinfo = QString::fromStdWString(task->name() + L":") +
				QObject::tr("Some disks lost, please replug the disks, and restart the program to reload the task.");
			CPMessageBox::warning(qinfo);
		}
		else {
			CPMessageBox::warning("unknown warning!");
		}
	});

	//锁定读写
	libdt::OneDevices sourceDevices;
	sourceDevices.push_back(param.sourceDevice);
	CPDeviceTaskUtils::setWritable(sourceDevices, false);
	libdt::OneDevices targetDevices;
	targetDevices.push_back(param.targetDevice);
	CPDeviceTaskUtils::setRW(targetDevices, false);
	task->addEndCallback([sourceDevices, targetDevices](CPTask* t) {
		//解锁读写
		CPDeviceTaskUtils::setWritable(sourceDevices, true);
		CPDeviceTaskUtils::setRW(targetDevices, true);
		g_cloneFolder->removeFlag(RFlagNoShow);
	});

	CPRecord* record = task->rootRecord();
	record->setFolder(false);
	g_cloneFolder->addChild(record);

	return task;
}

CPTask* CPDeviceTaskHandler::createImageTask(const devtask::DeviceImageParam& param,
	const std::wstring& taskId)
{
	//创建任务
	TaskParamPtr taskParam = std::make_shared<boost::any>(param);
	CPTask* task = CPTaskController::get()->createTask(L"task.deviceimage", taskParam);

	//锁定读写
	//进行镜像任务时，目标磁盘写锁定，这样可以多个镜像任务进行
	//但目标磁盘不能进行克隆、擦除
	libdt::OneDevices targetDevices;
    if (!CPFilePathChecker::getPathDevice(param.imagePath, targetDevices)) {
        return nullptr;
    }

	std::vector<OneDevice> sourceDevices;
	sourceDevices.push_back(param.sourceDevice);
	CPDeviceTaskUtils::setWritable(sourceDevices, false);
	CPDeviceTaskUtils::setWritable(targetDevices, false);
	task->addEndCallback([sourceDevices, targetDevices](CPTask* t) {
		//解锁读写
		CPDeviceTaskUtils::setWritable(sourceDevices, true);
		CPDeviceTaskUtils::setWritable(targetDevices, true);
		g_imageFolder->removeFlag(RFlagNoShow);
	});

	CPRecord* record = task->rootRecord();
	record->setFolder(false);
	g_imageFolder->addChild(record);
	return task;
}
