#pragma once
namespace app
{
	class NetProtocolStatisticsHelper //: public MemPipeParser
	{
	public:
		struct NetProtocolRecord
		{
			std::wstring entry_name_;
			std::wstring data_;
		};
		struct NetProtocolInformation
		{
			std::wstring protocol_name_;
			std::vector<NetProtocolRecord> net_protocol_info_;
		};
		typedef std::vector<NetProtocolInformation>  NetProtocolsInformation;
		struct ICMPRecord
		{
			std::wstring entry_name_;
			std::wstring sent_;
			std::wstring received_;
		};
		struct ICMPProtocolInformation
		{
			std::wstring protocol_name_;
			std::vector<ICMPRecord> icmp_protocol_info_;
		};
		typedef std::vector<ICMPProtocolInformation> ICMPProtocolsInformation;

		 bool analyze(CPRecord *rootRecord);

	private:
		 bool GetAllNetProtocolsStatisticsInformation(ICMPProtocolsInformation& icmp_protocols_info,
			NetProtocolsInformation& net_protocols_info);
		 bool ParseToNetProtocolsInformation(WCHAR* cmd_line,
			ICMPProtocolsInformation& icmp_protocols_info, NetProtocolsInformation& net_protocols_info);
		 uint32 icmp_protocols_count_;
		 uint32 net_protocols_count_;
	};
}
