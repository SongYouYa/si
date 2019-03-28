#include "stdafx.h"
#include "siutility/PDevicePath.h"

#include "sidevice/PDeviceManager.h"

#include "sicommon/PPath.h"

using namespace pdevice;

namespace siutil
{
	bool CPDevicePath::isSubPath(const std::wstring& rootPath, const std::wstring& destPath)
	{
		if (rootPath.empty() || destPath.empty()) {
			return false;
		}

		std::wstring tempRoot = rootPath;
		std::wstring tempDest = destPath;

		CPPath::formatPathSlash(tempRoot);
		CPPath::formatPathSlash(tempDest);
#ifdef OS_PWINDOWS
		boost::to_upper(tempRoot);
		boost::to_upper(tempDest);
#endif

#ifdef OS_PUNIX
		if (tempRoot == L"/") {
			return true;
		}
#endif

#ifdef OS_PWINDOWS
		if (tempRoot[tempRoot.size() - 1] != L'\\') {
			tempRoot += L'\\';
		}
		if (tempDest[tempDest.size() - 1] != L'\\') {
			tempDest += L'\\';
		}
#else
		if (tempRoot[tempRoot.size() - 1] != L'/') {
			tempRoot += L'/';
		}
		if (tempDest[tempDest.size() - 1] != L'/') {
			tempDest += L'/';
		}
#endif

		if (tempDest.find(tempRoot) == 0) {
			return true;
		}
		return false;
	}

	bool CPDevicePath::isInPartition(const std::wstring& partitionMount,
		const std::wstring& filePath)
	{
		if (partitionMount == L"/") {
			return false;
		}
		if (isSubPath(partitionMount, filePath)) {
			return true;
		}
		return false;
	}

	bool CPDevicePath::getPathDevice(const std::wstring& filePath,
		pdevice::DeviceDisk& disk,
		pdevice::DevicePartition& partition)
	{
#ifdef OS_PUNIX
        //linux:磁盘的分区挂载信息可能不是最新的，
        //这里重新更新下
        CPDeviceManager::get()->update(false);
#endif

		DeviceDisks allDisks;
		CPDeviceManager::get()->getAllDisks(allDisks);
		for (auto& d : allDisks) {
			for (auto& p : d.partitions) {
				for (auto& mount : p.mountPoints) {
					if (isInPartition(mount, filePath)) {
						disk = d;
						partition = p;
						return true;
					}
				}
			}
            //有些U盘没有分区，直接挂载的是磁盘，所以获取disk的挂载点
            for (auto& mount : d.mountPoints) {
                if (isInPartition(mount, filePath)) {
                    disk = d;
                    return true;
                }
            }
		}
        //logic partitions
        DevicePartitions logicPartitions;
        CPDeviceManager::get()->getAllLogicalPartitions(logicPartitions);
        for (auto& p : logicPartitions) {
            for (auto& mount : p.mountPoints) {
                if (isInPartition(mount, filePath)) {
                    partition = p;
                    return true;
                }
            }
        }

		return false;
	}

    bool CPDevicePath::getUsbDevice(const std::wstring& filePath,
        pdevice::DevicePartition& partition, pdevice::DeviceDisk& disk)
	{
#ifdef OS_PUNIX
        //linux:磁盘的分区挂载信息可能不是最新的，
        //这里重新更新下
        CPDeviceManager::get()->update(false);
#endif

		DeviceDisks usbDisks;
		CPDeviceManager::get()->getRemovableDisks(usbDisks);
		for (auto& d : usbDisks) {
			for (auto& p : d.partitions) {
				for (auto& mount : p.mountPoints) {
					if (isInPartition(mount, filePath)) {
						partition = p;
						return true;
					}
				}
			}
            //有些U盘没有分区，直接挂载的是磁盘，所以获取disk的挂载点
            for (auto& mount : d.mountPoints) {
                if (isInPartition(mount, filePath)) {
                    disk = d;
                    return true;
                }
            }
		}

        DevicePartitions remotePartitions;
        CPDeviceManager::get()->getRemoteLogicalPartitions(remotePartitions);
        for (auto& p : remotePartitions) {
            for (auto& mount : p.mountPoints) {
                if (isInPartition(mount, filePath)) {
                    partition = p;
                    return true;
                }
            }
        }
		return false;
	}
}
