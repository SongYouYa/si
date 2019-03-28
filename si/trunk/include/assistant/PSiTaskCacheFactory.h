#pragma once
#include "assistant/globals.h"
#include "task/PTaskCacheFactory.h"


class ASSISTANT_API CPSiTaskCacheFactory : public CPTaskCacheFactory
{
public:
	CPSiTaskCacheFactory(const std::wstring& caseDir);
	std::wstring rootDir();

protected:
	CPTaskCachePtr createCacheImpl(const std::wstring& taskType,
		const std::wstring& taskId,
		const std::wstring& cacheDir) override;

private:
	std::wstring _rootDir;
};
