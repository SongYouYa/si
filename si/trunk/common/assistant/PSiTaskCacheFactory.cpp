#include "stdafx.h"
#include "assistant/PSiTaskCacheFactory.h"

#include "application/DeviceTask/cache/PCloneTaskCache.h"
#include "application/DeviceTask/cache/PImageTaskCache.h"

CPSiTaskCacheFactory::CPSiTaskCacheFactory(const std::wstring& caseDir)
: CPTaskCacheFactory(caseDir)
{

}


CPTaskCachePtr CPSiTaskCacheFactory::createCacheImpl(const std::wstring& taskType,
	const std::wstring& taskId,
	const std::wstring& cacheDir)
{
	if (taskType == L"task.deviceclone") {
		return std::make_shared<devtask::CPCloneTaskCache>(taskId, cacheDir);
	}
	else if (taskType == L"task.deviceimage") {
		return std::make_shared<devtask::CPImageTaskCache>(taskId, cacheDir);
	}
	return nullptr;
}
