#pragma once
#include "ScheduleTasksAnalyzer.h"
namespace app
{
    class ListDirTasksHelper 
    {
    public:
		static bool analyze(CPRecord* rootRecord);

	private:
		static bool  GetWinDirTasksInformation(ScheduleTasksAnalyzer::WinDirTaskScheduleInformation& windir_task_info);
		static std::wstring FTToWstr(FILETIME &ft);
		static CPTime XpTimeStrToCPTime(const std::string& strTime); //转换xp系统下得到的时间字符串到cptime类
		
    };
}
