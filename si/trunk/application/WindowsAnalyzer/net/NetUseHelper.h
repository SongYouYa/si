#pragma once
namespace app
{
	class NetUseHelper
    {
    public:
		struct NetUseRecord
		{
			std::wstring status_;
			std::wstring local_;
			std::wstring remote_;
			std::wstring net_type_;               //  Õ¯¬Á¿‡–Õ
		};
		typedef std::vector<NetUseRecord> NetUseInformation;
        NetUseHelper();
        ~NetUseHelper();  
		static bool analyze(CPRecord* rootRecord);
    private:
		static bool  GetNetUseInformation(NetUseInformation& net_use_information);
    };
}
