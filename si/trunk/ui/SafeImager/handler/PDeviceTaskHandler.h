#pragma once

class CPDeviceTaskHandler
{
public:
	CPDeviceTaskHandler();
	//�������񣬲���������һЩ����

	static CPTask* createCloneTask(const devtask::DeviceCloneParam& param,
		const std::wstring& taskId = L"");
	static CPTask* createImageTask(const devtask::DeviceImageParam& param,
		const std::wstring& taskId = L"");
};
