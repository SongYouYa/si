#pragma once
#include <Iphlpapi.h>
namespace app
{
	class ARPInformationHelper //: public MemPipeParser
	{
	public:

		struct ARPInformation
		{
			std::wstring ip_address_;
			std::wstring physical_address_;
			std::wstring type_;
			std::wstring interface_;
		};
		typedef std::vector<ARPInformation> AllARPInformation;
		static bool analyze(CPRecord* rootRecord);

	private:
		static std::wstring ConvertDwordToIPString(uint32 ip);
		static bool InterfaceIndexToInterfaceIP(PMIB_IPADDRTABLE pIpAddrtable, DWORD dwIndex, DWORD &dwIpAddr);
		static PMIB_IPNETTABLE MyGetIpNetTable(BOOL bOrder);
		void MyFreeIpNetTable(PMIB_IPNETTABLE pIpNetTable);
		static PMIB_IPADDRTABLE  MyGetIpAddrTable(BOOL order);
		void MyFreeIpAddrTable(PMIB_IPADDRTABLE pIpAddrTable);
		static bool GetARPInformation(AllARPInformation& all_arp_information);
	};
}
