#pragma once
namespace app
{
	class   PromiscPortHelper
	{
	public:
		struct PromiscPortInformation
		{
			std::vector<std::wstring> adapters_;
		};
		static bool analyze(CPRecord* rootRecord);
		static void ShowAdaptersInfo();

	private:
		static  bool  GetPromiscPortInformation(PromiscPortInformation& promisc_port_information);
	};
}
