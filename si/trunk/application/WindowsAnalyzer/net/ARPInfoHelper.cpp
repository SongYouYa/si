#include "stdafx.h"
#include "ARPInfoHelper.h"

#pragma comment(lib, "Iphlpapi.lib")

namespace app
{
    std::wstring ARPInformationHelper::ConvertDwordToIPString(uint32 ip)
    {
        std::wstring ip_string;
        in_addr ia;
        ia.S_un.S_addr = ip;
        char* pIP = inet_ntoa(ia);
		ip_string =CPCodeChanger::UTF8ToUnicode(pIP).c_str();
        return ip_string;
    }
    bool  ARPInformationHelper::GetARPInformation(AllARPInformation& all_arp_information)
    {
		DWORD i = 0;
		DWORD dwCurrIndex = 1;
		DWORD dwIpAddr = 0;
		char szType[128] = {""};
		char temp[10] = "";
		ARPInformation one_arp_info;

		PMIB_IPNETTABLE pIpNetTable = MyGetIpNetTable(TRUE);
		PMIB_IPADDRTABLE pIpAddTable = MyGetIpAddrTable(TRUE);
		if ( InterfaceIndexToInterfaceIP(pIpAddTable,dwCurrIndex,dwIpAddr)){
			printf("\ninterface: %s On Interface 0x%X\n",inet_ntoa(*((in_addr*)&dwIpAddr)),dwCurrIndex);
		}
		for (i = 0;i < pIpNetTable->dwNumEntries;++i){ 
		   
			if (pIpNetTable->table[i].dwIndex != dwCurrIndex){
				dwCurrIndex = pIpNetTable->table[i].dwIndex;
				if (InterfaceIndexToInterfaceIP(pIpAddTable,dwCurrIndex,dwIpAddr)){
					char charinterface[50] = "";
                   sprintf_s(charinterface," %s---0x%x\n", inet_ntoa(*((in_addr*)&dwIpAddr)),dwCurrIndex);
				   one_arp_info.interface_ = CPCodeChanger::UTF8ToUnicode((std::string)charinterface);
				
				}
			}
			MIB_IPNETROW ipnet_row = pIpNetTable->table[i];
			UCHAR *phys_addr = ipnet_row.bPhysAddr;
			DWORD ip_addr = ipnet_row.dwAddr;
			DWORD type = ipnet_row.dwType;

			if (type != 3 && type != 4){
				continue;
			}
			one_arp_info.ip_address_ = ConvertDwordToIPString(ip_addr);

			if (ipnet_row.dwPhysAddrLen > 0){
				char stdphyaddr[30] = "";
				sprintf_s(stdphyaddr, "%02x", ipnet_row.bPhysAddr[0]);

				for (DWORD j = 1; j < ipnet_row.dwPhysAddrLen; j++){
					sprintf_s(temp, "-%02x", ipnet_row.bPhysAddr[j]);
					strcat_s(stdphyaddr, temp);
				}
				one_arp_info.physical_address_ = CPCodeChanger::UTF8ToUnicode((std::string)stdphyaddr);
			}
			else
			{
				continue;
			}
			switch (type)
			{
			case 1:
				one_arp_info.type_ = L"其它";
				break;

			case 2:
				one_arp_info.type_ = L"无效";
				break;

			case 3:
				one_arp_info.type_ = L"动态";
				break;

			case 4:
				one_arp_info.type_ = L"静态";
				break;

			default:
				one_arp_info.type_ = L"未知";
				break;
			}
			all_arp_information.push_back(one_arp_info);
		}    
        return true;
    }

	bool ARPInformationHelper::analyze(CPRecord* parentRecord)
    {
		std::vector<ARPInformation> allinfo;
		GetARPInformation(allinfo);
		if (allinfo.empty()) {
			return false;
		}
            
		for (auto it = allinfo.begin(); it != allinfo.end(); it++){
			ARPInformation& allinfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"IPAddress") = allinfo.ip_address_;
			record->at(L"PhyAddress") = allinfo.physical_address_;
			record->at(L"Type") = allinfo.type_;
			record->at(L"Interface") = allinfo.interface_;
		}
		return true;
    }

	bool ARPInformationHelper::InterfaceIndexToInterfaceIP( PMIB_IPADDRTABLE pIpAddrtable,DWORD dwIndex,DWORD &dwIpAddr )
	{
		for (DWORD dwIdx = 0;dwIdx <pIpAddrtable->dwNumEntries;dwIdx++){
			if (dwIndex == pIpAddrtable->table[dwIdx].dwIndex){
				dwIpAddr = pIpAddrtable->table[dwIdx].dwAddr;
				if (dwIpAddr != 0){
					return true;
				}
				else
					return false;
			}
		}
		return false;
	}

	PMIB_IPNETTABLE ARPInformationHelper::MyGetIpNetTable( BOOL bOrder )
	{
		PMIB_IPNETTABLE pIpNeTable =NULL;
		DWORD dwActualSize = 0;

		if (::GetIpNetTable(pIpNeTable,&dwActualSize,bOrder) == ERROR_INSUFFICIENT_BUFFER)
		{ //存储的变量不够大因此要重新申请
			pIpNeTable = (PMIB_IPNETTABLE)::GlobalAlloc(GPTR,dwActualSize);
			if (::GetIpNetTable(pIpNeTable,&dwActualSize,bOrder) == NO_ERROR)
			{
				return pIpNeTable;
			}

			::GlobalFree(pIpNeTable);
		}
		return NULL;
	}

	void ARPInformationHelper::MyFreeIpNetTable( PMIB_IPNETTABLE pIpNetTable )
	{
		if (pIpNetTable != NULL){
			::GlobalFree(pIpNetTable);
		}
	}

	PMIB_IPADDRTABLE ARPInformationHelper::MyGetIpAddrTable( BOOL bOrder )
	{
		PMIB_IPADDRTABLE pIpAddrTable = NULL;
		DWORD dwActualSize = 0;
		if (::GetIpAddrTable(pIpAddrTable,&dwActualSize,bOrder) == ERROR_INSUFFICIENT_BUFFER){
			pIpAddrTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR,dwActualSize);
			if (::GetIpAddrTable(pIpAddrTable,&dwActualSize,bOrder) == NO_ERROR){
				return pIpAddrTable;
			}
			::GlobalFree(pIpAddrTable);
		}
		return NULL;
		
	}

	void ARPInformationHelper::MyFreeIpAddrTable( PMIB_IPADDRTABLE pIpAddrTable )
	{
		if (pIpAddrTable != NULL){
			GlobalFree(pIpAddrTable);
		}

	}
}
