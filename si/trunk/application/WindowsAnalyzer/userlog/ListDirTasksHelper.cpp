#include "stdafx.h"
#include "ListDirTasksHelper.h"
#include <boost/tokenizer.hpp>
#include <boost/xpressive/xpressive.hpp>

namespace app
{
    std::wstring ListDirTasksHelper::FTToWstr(FILETIME &ft)
    {
        SYSTEMTIME st;
        FileTimeToSystemTime(&ft, &st);
        WCHAR tszBuffer[64];
        swprintf(tszBuffer,100, L"%04d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay,
                 st.wHour, st.wMinute, st.wSecond);
        return std::wstring(tszBuffer);
    }
	bool  ListDirTasksHelper::GetWinDirTasksInformation(ScheduleTasksAnalyzer::WinDirTaskScheduleInformation& windir_task_infol)
    {
    
        if (psystem::CPOsVersion::isVistaOrLater())
		{
			ScheduleTasksAnalyzer schAnaly;
			if (!schAnaly.ParseTaskInfo(windir_task_infol))
			{
				return false;
			}
        }
        else // xp
        {
			int exitCode = 0;
			//char* pipe_buf_ =nullptr;
			bool ret = CPProcess::createProcess(L"schtasks.exe", L"", NULL, &exitCode);
			if (!ret){
				return false;
			}    
			
			std::string string_record = "";
            size_t pos_first = 0;
            size_t pos_next = 0;
            const char *split = "\r\n";
            
            while (std::string::npos != (pos_next = string_record.find(split, pos_first)))
            {
                std::string strList(string_record, pos_first, pos_next - pos_first);
                pos_first = pos_next + strlen(split);
                strList.erase(0, strList.find_first_not_of("\r\n\t "));
                strList.erase(strList.find_last_not_of("\r\n\t ") + 1);
                if (strList.empty())  {
                    continue;
                }   
                if (std::string::npos != strList.find("INFO:") || std::string::npos != strList.find("信息:")){
                    continue;
                } 
                if (std::string::npos != strList.find("=====")){
                    continue;
                }           
                if (std::string::npos != strList.find("任务名")){
                    continue;
                }
                
                int offsets[]= {37, 25, 15};
                boost::offset_separator sep(offsets, offsets + 3, false);
                boost::tokenizer<boost::offset_separator> tok(strList, sep);
				ScheduleTasksAnalyzer::WinDirTaskRecord  task;
                boost::tokenizer<boost::offset_separator>::const_iterator pos = tok.begin();     
                if (pos != tok.end()){
                    std::string tmp = *pos;
					task.task_name_ = CPCodeChanger::UTF8ToUnicode(tmp); // trim
                    ++pos;
                }
                
                if (pos != tok.end()){
                    std::string tmp = *pos;
                    task.next_run_time_ = XpTimeStrToCPTime(tmp).ToString();
                    ++pos;
                }
                
                if (pos != tok.end()) {
                    std::string tmp = *pos;
					task.status_ = CPCodeChanger::UTF8ToUnicode(tmp);
                    ++pos;
                }      
				windir_task_infol.push_back(task);
            } // End of WHILE nPosNext
        }  
        return true;
    }

	bool ListDirTasksHelper::analyze(CPRecord* parentRecord)
	{
		ScheduleTasksAnalyzer::WinDirTaskScheduleInformation winDir_info;
		GetWinDirTasksInformation(winDir_info);
		if (winDir_info.empty()) {
			return false;
		}    
		for (auto it = winDir_info.begin(); it != winDir_info.end(); it++)
		{
			ScheduleTasksAnalyzer::WinDirTaskRecord& winDir_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"Taskname") = winDir_info.task_name_;
			record->at(L"NextRunTime") = winDir_info.next_run_time_;
			record->at(L"Status") = winDir_info.status_;
			record->at(L"DirName") = winDir_info.dir_name_;
		}
		return true;
	}

	CPTime ListDirTasksHelper::XpTimeStrToCPTime( const std::string& strTime )
	{
		CPTime cptime;
		const std::string strRegex = "(\\d{1,2}):(\\d{1,2}):(\\d{1,2}),\\s*(\\d{4})-(\\d{1,2})-(\\d{1,2})\\s*";
		const boost::xpressive::sregex reg = boost::xpressive::sregex::compile(strRegex);
		boost::xpressive::smatch what;
		if (boost::xpressive::regex_match(strTime, what, reg)){
			cptime.SetTime(std::strtol(what[4].str().c_str(), NULL, 10),
				std::strtol(what[5].str().c_str(), NULL, 10),
				std::strtol(what[6].str().c_str(), NULL, 10),
				std::strtol(what[1].str().c_str(), NULL, 10),
				std::strtol(what[2].str().c_str(), NULL, 10),
				std::strtol(what[3].str().c_str(), NULL, 10)
				);
		}
		return cptime;
	}

}
