#pragma once
namespace app
{
	class NetActiveStatHelper //: public MemPipeParser
	{
	public:
		struct NetActiveStatRecord
		{
			std::wstring protocol_;
			std::wstring local_address_;
			std::wstring foreign_address_;
			std::wstring state_;
		};
		typedef std::vector<NetActiveStatRecord> NetActiveStatInformation;
		static bool analyze(CPRecord* rootRecord);
	private:
		static bool GetNetActiveStatInformation(NetActiveStatInformation& net_active_stat);
	};
}
