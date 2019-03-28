#include "stdafx.h"
#include "NetSessionsHelper.h"
#include <boost/tokenizer.hpp>
namespace app
{
	bool NetSessionsHelper::GetNetSessionsStatInformation(NetSessionsInformation& net_sessions_information)
	{
		SYSTEMTIME system_time;
		GetSystemTime(&system_time);
		net_sessions_information.system_time_ = TimeConverter::SystemTimeToTimet(system_time);
		std::wstring cmdDir = siutil::CPDirHelper::get()->getToolDir() + L"\\cmd";
		WCHAR  cmd_line[MAX_PATH] = L"";
		swprintf(cmd_line, MAX_PATH, L"%s\\cmd.exe /c net.exe session", cmdDir.c_str());
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd_line, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		std::string string_record =  pipe_buf_;
		NetSessionsRecord session_record;
		size_t pos_first = 0;
		size_t pos_next = 0;
		const char *split = "\r\n";
		while (std::string::npos != (pos_next = string_record.find(split, pos_first)))
		{
			std::string strList(string_record, pos_first, pos_next - pos_first);
			pos_first = pos_next + strlen(split);
			strList.erase(0, strList.find_first_not_of("\r\n\t "));
			strList.erase(strList.find_last_not_of("\r\n\t ") + 1);

			if (strList.empty()){
				continue;
			}
			if ('\\' != strList.at(0)){
				continue;
			}

			int offsets[] = { 22, 22, 22, 22 };
			boost::offset_separator f(offsets, offsets + 4);
			boost::tokenizer<boost::offset_separator> tok(strList, f);
			boost::tokenizer<boost::offset_separator>::iterator it = tok.begin();
			std::string strTmp = *it;
			//  trim(strTmp);
			session_record.computer_ = CPCodeChanger::UTF8ToUnicode(strTmp);
			strTmp = *(++it);
			//  trim(strTmp);
			session_record.user_ = CPCodeChanger::UTF8ToUnicode(strTmp);
			strTmp = *(++it);
			//  trim(strTmp);
			session_record.client_type_ = CPCodeChanger::UTF8ToUnicode(strTmp);
			strTmp = *(++it);
			//  trim(strTmp);
			std::string opens_idletime = (std::string) strTmp;
			session_record.open_num_ = boost::lexical_cast<long>(opens_idletime.substr(0, 1));
			std::string idletime = opens_idletime.substr(2);
			session_record.idle_time_ = TimeConverter::TimerStringToInt(idletime);
			net_sessions_information.sessions_.push_back(session_record);
		}
		return true;
	}

	bool NetSessionsHelper::analyze(CPRecord* parentRecord)
	{
		NetSessionsInformation NetSession_info;
		if (!GetNetSessionsStatInformation(NetSession_info)) {
			return false;
		}
		if (NetSession_info.sessions_.empty()) {
			return true;
		}

		CPRecord* record = parentRecord->addChild();
		record->at(L"SystemTime") = NetSession_info.system_time_;
		record->at(L"Computer") = NetSession_info.sessions_.at(0).computer_;
		record->at(L"user") = NetSession_info.sessions_.at(0).user_;
		record->at(L"ClientType") = NetSession_info.sessions_.at(0).client_type_;
		record->at(L"OpemNum") = NetSession_info.sessions_.at(0).open_num_;
		record->at(L"IdleTime") = NetSession_info.sessions_.at(0).idle_time_;

		return true;
	}

}
