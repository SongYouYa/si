#include "stdafx.h"
#include "PFilePathChecker.h"

#include "libdevtask/PDeviceTaskUtils.h"
#include "uicommon/device/PDeviceUiUtils.h"

#include "siutility/PDevicePath.h"

using namespace pdevice;
using namespace libdt;

#define FAT_MAX_SIZE 4293918720U //4G减1M


bool CPFilePathChecker::checkTargetFreeSpace(const std::wstring& targetPath, uint64 sourceSize)
{
	//获取存放镜像磁盘的可以使用磁盘空间大小
	uint64 freeSpace = 0;
	try {
		boost::filesystem::path p(targetPath);
		p = p.parent_path();
		while (!boost::filesystem::exists(p)) {
			p = p.parent_path();
		}
		boost::filesystem::space_info space = boost::filesystem::space(p);
		freeSpace = space.available;
	}
	catch (...){
		CPMessageBox::warning(tr("Get Target disk size failure!"));
		return false;
	}
	//判断目标盘的大小
	if (sourceSize + 1 * 1024 * 1024 >= freeSpace){
		CPMessageBox::warning(tr("The Export mirror target diskcapacity is not enough"));
		return false;
	}
	return true;
}

bool CPFilePathChecker::checkTargetFsSize(const std::wstring& filesystem, uint64 fileSize)
{
	if (fileSize <= FAT_MAX_SIZE) {
		return true;
	}

    std::wstring fs = filesystem;
	boost::to_upper(fs);
	if (fs.find(L"FAT32") != std::string::npos ||
		fs.find(L"VFAT") != std::string::npos) {
		CPMessageBox::warning(tr("Target path filesystem can not surport file size larger than 4G!"));
		return false;
	}
	return true;
}

bool CPFilePathChecker::checkTargetPath(const std::wstring& imagePath,
	uint64 totalSize, uint64 singleSize)
{
	DevicePartition partition;
    DeviceDisk disk;
    if (!siutil::CPDevicePath::getUsbDevice(imagePath, partition, disk)) {
		//检测是否在USB路径
		CPMessageBox::warning(tr("Please choose usb path!"));
		return false;
	}

	bool readOnly = false;
    std::wstring fs;
    if (!partition.name.empty()) {
        fs = partition.filesystem;
		readOnly = partition.readOnly;
    }
    else if (!disk.name.empty()) {
        fs = disk.filesystem;
		readOnly = disk.readOnly;
    }

	if (readOnly) {
		CPMessageBox::warning(tr("Target device is read only!"));
		return false;
	}

	if (singleSize == 0) {
		singleSize = totalSize;
	}
    if (!checkTargetFsSize(fs, singleSize)) {
		return false;
	}

	if (!checkTargetFreeSpace(imagePath, totalSize)) {
		return false;
	}

	return true;
}

bool CPFilePathChecker::getPathDevice(const std::wstring& filePath,
	libdt::OneDevices& targetDevices)
{
	DeviceDisk disk;
	DevicePartition partition;
	if (!siutil::CPDevicePath::getPathDevice(filePath, disk, partition)) {
		CPMessageBox::warning(tr("Get dest disk error!"));
		return false;
	}
	OneDevice targetDievice;
	targetDievice.deviceName = disk.name;
	targetDievice.isPartition = false;
	targetDevices.push_back(targetDievice);
	return true;
}
