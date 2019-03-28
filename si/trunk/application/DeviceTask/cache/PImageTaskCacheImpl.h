#pragma once
#include "siio/PDeviceWriter.h"
#include "libdevtask/cache/PHashCache.h"
#include "libdevtask/cache/PTaskProgressCache.h"

namespace devtask
{
	class CPImageTaskCacheImpl
	{
	public:
		CPImageTaskCacheImpl(const std::wstring& cacheDir);
		bool oopen(const DeviceImageParam& imageParam, libdt::CPHashComputerPtr hasher);
		bool iopen();

		DeviceImageParam getParam() {
			return _imageParam;
		}
		libdt::CPHashComputerPtr getHasher() {
			return _hasher;
		}
		void setWriter(siio::CPFilePtr writer) {
			std::lock_guard<std::mutex> lock(_mutex);
			_writer = writer;
		}

		void setProgress(CPTaskProgress* progress) { 
			_progress = progress; 
		}
		void getProgressInfo(libdt::ProgressInfo& progressInfo);

		//¸üÐÂ»º´æ
		void update();

		void close();

	private:
		std::wstring _cacheDir;

		DeviceImageParam _imageParam;
		libdt::CPHashComputerPtr _hasher;
		siio::CPFilePtr _writer;
		libdt::CPHashCaches _hashCaches;

		CPTaskProgress* _progress;
		libdt::CPProgressCachePtr _progressCache;

		std::mutex _mutex;
	};
}