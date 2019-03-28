#pragma once
#include "application/DeviceTask/globals.h"
#include "task/PTaskCache.h"

namespace devtask
{
	class CPImageTaskCacheImpl;
	class DEV_TASK_API CPImageTaskCache : public CPTaskCache
	{
	public:
		CPImageTaskCache(const std::wstring& dirName,
			const std::wstring& cacheDir);
		~CPImageTaskCache();

		TaskParamPtr getParam() override;

		CPImageTaskCacheImpl* getImpl();

		void clear() override;

	protected:
		void updateCache() override;

	private:
		CPImageTaskCacheImpl* _impl;
	};
}
