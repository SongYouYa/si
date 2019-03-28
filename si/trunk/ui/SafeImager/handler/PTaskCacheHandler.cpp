#include "stdafx.h"
#include "PTaskCacheHandler.h"

#include "assistant/PSiTaskCacheFactory.h"
#include "PDeviceTaskHandler.h"

extern CPTabManager* g_mainTab;

CPTaskCacheHandler::CPTaskCacheHandler()
{

}

void CPTaskCacheHandler::handleCachedTasks(bool prompt)
{
	std::vector<CPTaskCachePtr> allCaches;
	CPTaskController::get()->getCacheFactory()->getAllCaches(allCaches);
	if (allCaches.empty()) {
		return;
	}

	if (prompt &&
		CPDialog::Accepted != CPMessageBox::question(tr("reload the cached tasks?"))) {
		return;
	}

	g_mainTab->navToTab(PTAB_TASK);//切换页面

	bool failed = false;
	for (auto taskCache : allCaches) {
		if (!taskCache) {
			//可能有些cache目录，没有成功创建cache，这里就会为空
			failed = true;
			continue;
		}
		std::wstring taskType = taskCache->getTaskType();
		TaskParamPtr param = taskCache->getParam();
		CPTask* task = createTask(taskType, taskCache->dirName(), param);
		if (task) {
			task->setCache(taskCache);
			CPTaskController::get()->doTask(task);
		}
		else {
			failed = true;
		}
	}
	if (failed) {
		CPMessageBox::warning(tr("Some tasks reload failed!"));
	}
}

CPTask* CPTaskCacheHandler::createTask(const std::wstring& taskType,
	const std::wstring& taskId,
	TaskParamPtr param)
{
    if (taskType == L"task.deviceclone" && param) {
		devtask::DeviceCloneParam cloneParam;
		try {
			cloneParam = boost::any_cast<devtask::DeviceCloneParam>(*param);
		}
		catch (...) {
			PLOG(PLOG_ERROR, L"克隆任务：参数错误");
			return nullptr;
		}
		return CPDeviceTaskHandler::createCloneTask(cloneParam, taskId);
	}
	else if (taskType == L"task.deviceimage" && param) {
		devtask::DeviceImageParam imageParam;
		try {
			imageParam = boost::any_cast<devtask::DeviceImageParam>(*param);
		}
		catch (...) {
			PLOG(PLOG_ERROR, L"克隆任务：参数错误");
			return nullptr;
		}
		return CPDeviceTaskHandler::createImageTask(imageParam, taskId);
	}
	
	return nullptr;
}
