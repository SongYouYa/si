#pragma once
#include "application/DeviceTask/globals.h"

namespace devtask
{
	class DEV_TASK_API CPDeviceCloneTask
	{
	public:
		CPDeviceCloneTask();
		~CPDeviceCloneTask();
		void doTask(CPTask* task);

	private:
		class Impl;
		Impl* _impl;
	};
}
