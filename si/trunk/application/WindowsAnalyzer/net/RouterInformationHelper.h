#pragma once
namespace app
{
	class  RouterInformationHelper
	{
	public:
		struct RouterInformation
		{
			std::wstring dest_address_;
			std::wstring net_mask_;
			std::wstring gate_way_;
			std::wstring interface_;
			std::wstring metric_;
			std::wstring type_;
		};
		typedef std::vector<RouterInformation> AllRouterInformation;
		RouterInformationHelper();
		~RouterInformationHelper();
		static bool analyze(CPRecord* rootRecord);

	private:
		static void GetPermanentRoute(AllRouterInformation& all_router_information);
		static  void GetAdaptersInformation(std::map<uint32, std::wstring>& interface_info);
		static std::wstring ConvertDwordToIPString(uint32 ip);
		static bool GetRouterInformation(AllRouterInformation& all_router_information);
	};
}
