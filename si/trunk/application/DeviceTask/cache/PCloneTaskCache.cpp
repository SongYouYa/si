#include "stdafx.h"
#include "application/DeviceTask/cache/PCloneTaskCache.h"
#include "PCloneTaskCacheImpl.h"


namespace devtask
{
	CPCloneTaskCache::CPCloneTaskCache(const std::wstring& dirName,
		const std::wstring& cacheDir)
		: CPTaskCache(dirName, cacheDir),
		_impl(new CPCloneTaskCacheImpl(cacheDir))
	{
		_taskType = L"task.deviceclone";
	}

	CPCloneTaskCache::~CPCloneTaskCache()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	TaskParamPtr CPCloneTaskCache::getParam()
	{
		if (!_impl->iopen()) {
			return nullptr;
		}
		DeviceCloneParam cloneParam = _impl->getParam();
		return std::make_shared<boost::any>(cloneParam);
	}

	CPCloneTaskCacheImpl* CPCloneTaskCache::getImpl()
	{
		return _impl;
	}

	void CPCloneTaskCache::updateCache()
	{
		_impl->update();
	}

	void CPCloneTaskCache::clear()
	{
		_impl->close();
		CPTaskCache::clear();
	}
}