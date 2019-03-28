#include "stdafx.h"
#include "NetBiosStatHelper.h"
namespace app
{
	bool  NetBiosStatHelper::GetNetBiosStatInformation(NetBiosInformation& net_bios_information)
	{
		WCHAR cmd_line[MAX_PATH] = L"";
		std::wstring cmdDir = siutil::CPDirHelper::get()->getToolDir() + L"\\cmd";
		swprintf(cmd_line, MAX_PATH, L"%s\\cmd.exe /c nbtstat /c", cmdDir.c_str());
		int exitCode = 0;
		std::string pipe_buf_;
		if (!CPProcess::createProcess(cmd_line, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		std::string string_record = pipe_buf_;
		NetBiosInfoOfOneNode single_record;
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

			if (std::string::npos != strList.find(':'))
			{
				if (single_record.node_ip_ != L"" && single_record.node_name_ != L"")
				{
					net_bios_information.push_back(single_record);
					single_record.node_name_ = L"";
					single_record.node_ip_ = L"";
					single_record.bios_records_.clear();
				}
				size_t pos_split_begin = (size_t)0;
				size_t pos_split_end = (size_t)0;;
				if (single_record.node_name_ == L""){
					//Node name Row
					pos_split_begin = (size_t)0;
					pos_split_end = strList.find_first_of(':');
					// Node name
					single_record.node_name_ = CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
				}
				if (single_record.node_ip_ == L""){
					// Node IP Row
					pos_split_begin = strList.find_first_of('[');
					pos_split_end = strList.find_first_of(']');
					if (std::string::npos != pos_split_begin){
						single_record.node_ip_ = CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin + 1, pos_split_end - pos_split_begin - 1));
					}
				}
			}
			else  if (std::string::npos != strList.find('.'))
			{
				//NetBIOS Remote Cache Name Table Record  Row
				size_t pos_split_begin = strList.find_first_not_of(' ');
				size_t pos_split_end = strList.find_first_of(' ', pos_split_begin);
				// name
				NetBiosRecord record;
				record.name_ = CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
				//type
				pos_split_begin = strList.find_first_of('<');
				pos_split_end = strList.find_first_of(' ', pos_split_begin);
				std::wstring type = CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
				pos_split_begin = strList.find_first_not_of(' ', pos_split_end);
				pos_split_end = strList.find_first_of(' ', pos_split_begin);
				type.append(CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin)).c_str());
				record.type_ = type;
				// host address
				pos_split_begin = strList.find_first_not_of(' ', pos_split_end);
				pos_split_end = strList.find_first_of(' ', pos_split_begin);
				record.host_address_ = CPCodeChanger::GBToUnicode(strList.substr(pos_split_begin, pos_split_end - pos_split_begin));
				// life
				pos_split_begin = strList.find_first_not_of(' ', pos_split_end);
				pos_split_end = strList.find_first_of(' ', pos_split_begin);
				std::string life_str = strList.substr(pos_split_begin, pos_split_end - pos_split_begin);
				record.life_ = boost::lexical_cast<long>(life_str.c_str());
				single_record.bios_records_.push_back(record);
			}
			else
			{
				continue;
			}
		} // End of WHILE nPosNext

		if (single_record.node_ip_ != L"")
		{
			net_bios_information.push_back(single_record);
		}
		return true;
	}

	bool NetBiosStatHelper::analyze(CPRecord* parentRecord)
	{
		NetBiosInformation  bios_info;
		if (!GetNetBiosStatInformation(bios_info)) {
			return false;
		}

		for (auto it = bios_info.begin(); it != bios_info.end(); ++it) {
			NetBiosInfoOfOneNode& bios_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"NodeName") = bios_info.node_name_;
			record->at(L"NodeIP") = bios_info.node_ip_;
			if (!bios_info.bios_records_.size()){
				 continue;
			}
			for (auto&biosrecord : bios_info.bios_records_){
				record->at(L"Name") = biosrecord.name_;
				record->at(L"Type") = biosrecord.type_;
				record->at(L"HostAddress") = biosrecord.host_address_;
				record->at(L"Life") = biosrecord.life_;
			}
		}
		return true;
	}
}
