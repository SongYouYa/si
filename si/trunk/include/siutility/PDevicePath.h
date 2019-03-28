#pragma once
#include "siutility/globals.h"

namespace siutil
{
	class SI_UTILITY_API CPDevicePath
	{
	public:
		static bool isSubPath(const std::wstring& rootPath, const std::wstring& destPath);
		static bool getPathDevice(const std::wstring& filePath,
			pdevice::DeviceDisk& disk,
			pdevice::DevicePartition& partition);
         //有些U盘没有分区，直接挂载的是磁盘，所以获取的也可能是disk
		//先检查partition，再检查disk
        static bool getUsbDevice(const std::wstring& filePath,
                                 pdevice::DevicePartition& partition,
                                 pdevice::DeviceDisk& disk);

	private:
		static bool isInPartition(const std::wstring& partitionMount,
			const std::wstring& filePath);
	};
}
