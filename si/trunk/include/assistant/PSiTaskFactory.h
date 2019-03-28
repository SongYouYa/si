#pragma once
#include "assistant/globals.h"
#include "task/PTaskFactory.h"


class ASSISTANT_API CPSiTaskFactory : public CPTaskFactory
{

public:
	void getTaskLog(CPTask* task, std::wstring& strLog) override;

protected:
	void getTaskParams(const std::wstring& taskType,
		const TaskParamPtr param,
		CPRecord* record,
		std::wstring& paramStr,
		TaskCallback& callback,
		CPTaskDelegatePtr& taskDelegate);

private:
	class Impl;
};
