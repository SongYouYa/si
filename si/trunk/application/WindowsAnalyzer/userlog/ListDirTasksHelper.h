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
		static CPTime XpTimeStrToCPTime(const std::string& strTime); //ת��xpϵͳ�µõ���ʱ���ַ�����cptime��
		
    };
}
