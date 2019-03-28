#pragma once
#include "application/DeviceTask/globals.h"

namespace devtask
{
	class DEV_TASK_API CPExportFilesTask
	{
	public:
		CPExportFilesTask();
		~CPExportFilesTask();
		void doTask(CPTask* task);

	private:
		class Impl;
		Impl* _impl;
	};
}
