#pragma once
namespace app
{
	class NetSessionsHelper //: public MemPipeParser
	{
		struct NetSessionsRecord
		{
			std::wstring computer_;
			std::wstring user_;
			std::wstring client_type_;
			uint32 open_num_;
			boost::int64_t idle_time_;
		};
		struct NetSessionsInformation
		{
			boost::int64_t system_time_;
			std::vector<NetSessionsRecord> sessions_;
		};
		
	public:
		static bool analyze(CPRecord* rootRecord);
	private:
		static bool  GetNetSessionsStatInformation(NetSessionsInformation& net_sessions_information);
	};
}
