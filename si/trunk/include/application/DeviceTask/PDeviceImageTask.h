#pragma once
#include "application/DeviceTask/globals.h"

namespace devtask
{
	class DEV_TASK_API CPDeviceImageTask
	{
	public:
		CPDeviceImageTask();
		void doDeviceTask(CPTask* task);
		void doMemoryTask(CPTask* task);
	};
}
