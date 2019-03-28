#pragma once
#include "siio/PDeviceWriter.h"
#include "libdevtask/cache/PHashCache.h"
#include "libdevtask/cache/PTaskProgressCache.h"

namespace devtask
{
	class CPCloneTaskCacheImpl
	{
	public:
		CPCloneTaskCacheImpl(const std::wstring& cacheDir);
		bool oopen(const DeviceCloneParam& cloneParam, libdt::CPHashComputerPtr hasher);
		bool iopen();

		DeviceCloneParam getParam() {
			return _cloneParam;
		}
		libdt::CPHashComputerPtr getHasher() {
			return _hasher;
		}
		void setWriter(siio::CPDeviceWriterPtr writer) {
			_writer = writer;
		}

		void setProgress(CPTaskProgress* progress) { 
			_progress = progress; 
		}
		void getProgressInfo(libdt::ProgressInfo& progressInfo);

		//更新缓存
		void update();

		void close();

	private:
		//在硬盘最后写入guid
		static bool writeTargetGuid(siio::CPDeviceWriterPtr writer, std::string& guid);
		//读取磁盘最后的guid
		static bool getDiskGuid(const pdevice::DeviceDisk& disk, std::string& guid);
		//根据磁盘ID和guid来找到目标盘
		static bool getTargetDisk(const pdevice::DeviceId& targetId, 
			const std::string& targetGuid, libdt::OneDevice& targetDisk);

		std::wstring _cacheDir;

		DeviceCloneParam _cloneParam;
		libdt::CPHashComputerPtr _hasher;
		siio::CPDeviceWriterPtr _writer;
		libdt::CPHashCaches _hashCaches;

		CPTaskProgress* _progress;
		libdt::CPProgressCachePtr _progressCache;

		std::mutex _mutex;
	};
}