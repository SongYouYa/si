#include "stdafx.h"
#include "NetActiveStatHelper.h"

namespace app
{
	bool  NetActiveStatHelper::GetNetActiveStatInformation(NetActiveStatInformation& net_active_stat)
	{
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(L"netstat /n", L"", &pipe_buf_, &exitCode)){
			return false;
		}
		std::string string_record = pipe_buf_;
		NetActiveStatRecord record;
		size_t pos_first = 0;
		size_t pos_next = 0;
		const char *split = "\r\n";

		while (std::string::npos != (pos_next = string_record.find(split, pos_first)))
		{
			std::string strList(string_record, pos_first, pos_next - pos_first);
			pos_first = pos_next + strlen(split);
			strList.erase(0, strList.find_first_not_of("\r\n\t "));
			strList.erase(strList.find_last_not_of("\r\n\t ") + 1);

			if (strList.empty())
			{
				continue;
			}

			if (std::string::npos == strList.find_first_of("0123456789"))
			{
				continue;
			}

			size_t pos_split_begin = (size_t)0;
			size_t pos_split_end = strList.find_first_of(' ');
			// protocol
			record.protocol_ = CPCodeChanger::UTF8ToUnicode(strList.substr(pos_split_begin, pos_split_end));
			// local address
			pos_split_begin = strList.find_first_not_of(' ', pos_split_end + 1);
			pos_split_end = strList.find_first_of(' ', pos_split_begin + 1);
			record.local_address_ = CPCodeChanger::UTF8ToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
			// physical address
			pos_split_begin = strList.find_first_not_of(' ', pos_split_end + 1);
			pos_split_end = strList.find_first_of(' ', pos_split_begin + 1);
			record.foreign_address_ = CPCodeChanger::UTF8ToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
			//status
			pos_split_begin = strList.find_first_not_of(' ', pos_split_end + 1);
			pos_split_end = strList.find_first_of(' ', pos_split_begin + 1);
			record.state_ = CPCodeChanger::UTF8ToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
			net_active_stat.push_back(record);
		} // End of WHILE nPosNext


		return true;
	}
	bool NetActiveStatHelper::analyze(CPRecord *parentRecord)
	{
		NetActiveStatInformation  Active_info;
		GetNetActiveStatInformation(Active_info);
		if (Active_info.empty()) {
			return false;
		}

		for (auto it = Active_info.begin(); it != Active_info.end(); it++)
		{
			NetActiveStatRecord& Active_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"Protocal") = Active_info.protocol_;
			record->at(L"LocalIP") = Active_info.local_address_;
			record->at(L"RemoteIP") = Active_info.foreign_address_;
			record->at(L"State") = Active_info.state_;
		}
		return true;

	}
}
