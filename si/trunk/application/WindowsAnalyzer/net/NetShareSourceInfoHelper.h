#pragma once
namespace app
{
    class NetShareSourceInfoHelper
    {
    public:
		struct  NetShareSourceInformation
		{
			std::wstring net_name_;
			std::wstring path_;
			std::wstring current_uses_;
			std::wstring security_descriptor_;
			std::wstring password_;
		};
		typedef std::vector<NetShareSourceInformation> NetShareSourcesInformation;

		static bool analyze(CPRecord* rootRecord);
	private:
		static bool  GetNetShareSourceInformation(NetShareSourcesInformation& share_sources_info);
    };
}