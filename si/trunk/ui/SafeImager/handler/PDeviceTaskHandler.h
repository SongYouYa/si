#pragma once

class CPDeviceTaskHandler
{
public:
	CPDeviceTaskHandler();
	//创建任务，并对任务做一些处理

	static CPTask* createCloneTask(const devtask::DeviceCloneParam& param,
		const std::wstring& taskId = L"");
	static CPTask* createImageTask(const devtask::DeviceImageParam& param,
		const std::wstring& taskId = L"");
};
