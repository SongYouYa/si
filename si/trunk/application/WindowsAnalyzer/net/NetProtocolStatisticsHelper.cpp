#include "stdafx.h"
#include "NetProtocolStatisticsHelper.h"
#include <stdio.h>
#include <wchar.h>

namespace app
{
    bool NetProtocolStatisticsHelper::GetAllNetProtocolsStatisticsInformation(ICMPProtocolsInformation& icmp_protocols_info, NetProtocolsInformation& net_protocols_info)
    {
        WCHAR  cmd_line[256] = L"";// L"cmd.exe /c netstat.exe /s";
        swprintf(cmd_line, 100,L"netstat.exe /s");
        
        if (!ParseToNetProtocolsInformation(cmd_line, icmp_protocols_info, net_protocols_info))
		{
            return false;
        }   
        return true;
    }
    bool NetProtocolStatisticsHelper::ParseToNetProtocolsInformation(WCHAR* cmd_line, ICMPProtocolsInformation& icmp_protocols_info, NetProtocolsInformation& net_protocols_info)
    {
        icmp_protocols_count_ = 0;
        net_protocols_count_ = 0;
        
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd_line, L"", &pipe_buf_, &exitCode)){
			return false;
		}   
		std::string record = pipe_buf_;
        bool is_icmp = false;
        size_t posfirst = 0;
        size_t posnext = 0;
        const char *split = "\r\n";
        
        while (std::string::npos != (posnext = record.find(split, posfirst)))
        {
            std::string string_list(record, posfirst, posnext - posfirst);
            posfirst = posnext + strlen(split);
            string_list.erase(0, string_list.find_first_not_of("\r\n\t "));
            string_list.erase(string_list.find_last_not_of("\r\n\t ") + 1);
            
            if (string_list.empty())
            {
                continue;
            }
            
            if (std::string::npos != string_list.find("icmp") ||
                    std::string::npos != string_list.find("ICMP"))
            {
                is_icmp = true;
                ICMPProtocolInformation new_icmp;
				new_icmp.protocol_name_ = CPCodeChanger::GBToUnicode(string_list);
                icmp_protocols_info.push_back(new_icmp);
                icmp_protocols_count_++;
                continue;
            }
            
            if (std::string::npos != string_list.find("ip") ||
                    std::string::npos != string_list.find("IP"))
            {
                is_icmp = false;
                NetProtocolInformation new_net_protocol;
				new_net_protocol.protocol_name_ = CPCodeChanger::GBToUnicode(string_list);
                net_protocols_info.push_back(new_net_protocol);
                net_protocols_count_++;
                continue;
            }
            
            // ICMP Statistics
            if (is_icmp)
            {
#ifdef _DEBUG
                size_t stTest = string_list.find_last_of("0123456789");
#endif // _DEBUG
                
                if (string_list.length() - 1 != string_list.find_last_of("0123456789"))
                {
                    continue;
                }
                
                std::reverse(string_list.begin(), string_list.end());
                size_t pos_split_begin = (size_t)0;
                size_t pos_split_end = string_list.find(' ');
                ICMPRecord new_icmp_record;
                // sent
                std::string tmp_sent = string_list.substr(pos_split_begin, pos_split_end - pos_split_begin);
                std::reverse(tmp_sent.begin(), tmp_sent.end());
				new_icmp_record.sent_ = CPCodeChanger::GBToUnicode(tmp_sent);
                // recv
                pos_split_begin = string_list.find_first_not_of(' ', pos_split_end + 1);
                pos_split_end = string_list.find_first_of(' ', pos_split_begin + 1);
                std::string tmp_recv = string_list.substr(pos_split_begin, pos_split_end - pos_split_begin);
                std::reverse(tmp_recv.begin(), tmp_recv.end());
				new_icmp_record.received_ = CPCodeChanger::GBToUnicode(tmp_recv);
                // Name
                pos_split_begin = string_list.find_first_not_of(' ', pos_split_end + 1);
                std::string tmp_name = string_list.substr(pos_split_begin);
                std::reverse(tmp_name.begin(), tmp_name.end());
				new_icmp_record.entry_name_ = CPCodeChanger::GBToUnicode(tmp_name);
                icmp_protocols_info.at(icmp_protocols_count_-1).icmp_protocol_info_.push_back(new_icmp_record);
            }
            // Other
            else
            {
                size_t npos_split = string_list.find('=');
                
                if (std::string::npos == npos_split)
                {
                    // new_netprotocol.protocol_name_ = CPCodeChanger::UTF8ToUnicode(trim(string_list));
                    // newNetstats.wstrValue = L"";
                }
                else
                {
					NetProtocolRecord new_netprotocol_record;
					new_netprotocol_record.entry_name_ = LS2W(string_list.substr(0, npos_split));
					new_netprotocol_record.data_ = LS2W(string_list.substr(npos_split + 1));
					net_protocols_info.at(net_protocols_count_ - 1).net_protocol_info_.push_back(new_netprotocol_record);
                }
            }
        } // End of WHILE nPosNext
        
        return true;
    }

	bool NetProtocolStatisticsHelper::analyze(CPRecord *parentRecord)
	{
		NetProtocolsInformation  NetProtocol_info;
		ICMPProtocolsInformation icmp_protocols_info;

		if (!GetAllNetProtocolsStatisticsInformation(icmp_protocols_info,NetProtocol_info))
		{
			return false;
		}
		for (auto it = NetProtocol_info.begin(); it != NetProtocol_info.end(); ++it )
		{
			NetProtocolInformation& netInfo = *it;
			for (auto it_record = netInfo.net_protocol_info_.begin(); it_record != netInfo.net_protocol_info_.end(); ++it_record)
			{
				CPRecord* accessRecord = parentRecord->addChild();
				accessRecord->at(L"ProtocolName") = netInfo.protocol_name_;
				accessRecord->at(L"EntryName") = it_record->entry_name_;
				accessRecord->at(L"data") = it_record->data_;
				//...²»È«
			}
		}

		for (auto it_icmp = icmp_protocols_info.begin(); it_icmp != icmp_protocols_info.end(); ++it_icmp)
		{
			ICMPProtocolInformation& icmpInfo = *it_icmp;
			for (auto it_icmp_re = icmpInfo.icmp_protocol_info_.begin(); it_icmp_re != icmpInfo.icmp_protocol_info_.end(); ++it_icmp_re)
			{
				CPRecord* icmpRecord = parentRecord->addChild();
				icmpRecord->at(L"ProtocolName") = icmpInfo.protocol_name_;
				icmpRecord->at(L"EntryName") = it_icmp_re->entry_name_;
				icmpRecord->at(L"Sent") = it_icmp_re->sent_;
				icmpRecord->at(L"Received") = it_icmp_re->received_;
			}
		}
		return true;
	}

}
