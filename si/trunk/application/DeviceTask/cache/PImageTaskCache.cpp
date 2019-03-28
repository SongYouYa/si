#include "stdafx.h"
#include "application/DeviceTask/cache/PImageTaskCache.h"
#include "PImageTaskCacheImpl.h"


namespace devtask
{
	CPImageTaskCache::CPImageTaskCache(const std::wstring& dirName,
		const std::wstring& cacheDir)
		: CPTaskCache(dirName, cacheDir),
		_impl(new CPImageTaskCacheImpl(cacheDir))
	{
		_taskType = L"task.deviceimage";
	}

	CPImageTaskCache::~CPImageTaskCache()
	{
		if (_impl) {
			delete _impl;
			_impl = nullptr;
		}
	}

	TaskParamPtr CPImageTaskCache::getParam()
	{
		if (!_impl->iopen()) {
			return nullptr;
		}
		DeviceImageParam imageParam = _impl->getParam();
		return std::make_shared<boost::any>(imageParam);
	}

	CPImageTaskCacheImpl* CPImageTaskCache::getImpl()
	{
		return _impl;
	}

	void CPImageTaskCache::updateCache()
	{
		_impl->update();
	}

	void CPImageTaskCache::clear()
	{
		_impl->close();
		CPTaskCache::clear();
	}
}