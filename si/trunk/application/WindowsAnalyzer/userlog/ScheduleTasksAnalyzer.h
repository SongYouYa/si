#pragma once
#include <taskschd.h>

namespace app
{
	class ScheduleTasksAnalyzer
	{
	public:
		struct WinDirTaskRecord
		{
			std::wstring task_name_;
			std::wstring next_run_time_;
			std::wstring status_;
			std::wstring dir_name_;
		};
		typedef std::vector<WinDirTaskRecord> WinDirTaskScheduleInformation;

		ScheduleTasksAnalyzer();
		~ScheduleTasksAnalyzer();

		bool ParseTaskInfo(WinDirTaskScheduleInformation& taskInfos);
	private:
		bool Initialize(ITaskFolder*& pRootFolder);

		bool ParseFolder(ITaskFolder* pFolder, WinDirTaskScheduleInformation& taskInfos);
		void ParseTaskCollection(IRegisteredTaskCollection* pTaskCollection, WinDirTaskScheduleInformation& taskInfos);

		std::wstring GetTaskStatusStr(TASK_STATE state);
	};
}
