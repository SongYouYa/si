#pragma once
#include "application/DeviceTask/globals.h"
#include "task/PTaskCache.h"

namespace devtask
{
	class CPCloneTaskCacheImpl;
	class DEV_TASK_API CPCloneTaskCache : public CPTaskCache
	{
	public:
		CPCloneTaskCache(const std::wstring& dirName,
			const std::wstring& cacheDir);
		~CPCloneTaskCache();

		TaskParamPtr getParam() override;

		CPCloneTaskCacheImpl* getImpl();

		void clear() override;

	protected:
		void updateCache() override;

	private:
		CPCloneTaskCacheImpl* _impl;
	};
}
