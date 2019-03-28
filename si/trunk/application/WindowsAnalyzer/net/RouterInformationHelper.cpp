#include "stdafx.h"
#include "RouterInformationHelper.h"
#include <Iphlpapi.h>
#include <WinSock.h>

namespace app
{
	RouterInformationHelper::RouterInformationHelper()
	{
	}
	RouterInformationHelper:: ~RouterInformationHelper()
	{
	}
	bool  RouterInformationHelper::GetRouterInformation(AllRouterInformation& all_router_information)
	{
		std::map<uint32, std::wstring> adapter_interface;
		adapter_interface.clear();
		GetAdaptersInformation(adapter_interface);
		PMIB_IPFORWARDTABLE ip_forward_table_ptr = NULL;
		DWORD size = 0;
		DWORD return_value = 0;

		//探测路由表所需的存储空间
		if (GetIpForwardTable(ip_forward_table_ptr, &size, 0) == ERROR_INSUFFICIENT_BUFFER){
			GlobalFree(ip_forward_table_ptr);
			ip_forward_table_ptr = (MIB_IPFORWARDTABLE*)malloc(size);
		}
		//获取路由信息
		if ((return_value = GetIpForwardTable(ip_forward_table_ptr, &size, 0)) == NO_ERROR){
			for (size_t i = 0; i < ip_forward_table_ptr->dwNumEntries; i++){
				RouterInformation router_info;
				std::string metric = boost::lexical_cast<std::string>(ip_forward_table_ptr->table[i].dwForwardMetric1);
				router_info.dest_address_ = ConvertDwordToIPString(ip_forward_table_ptr->table[i].dwForwardDest);
				router_info.net_mask_ = ConvertDwordToIPString(ip_forward_table_ptr->table[i].dwForwardMask);
				router_info.gate_way_ = ConvertDwordToIPString(ip_forward_table_ptr->table[i].dwForwardNextHop);
				std::map<uint32, std::wstring> ::const_iterator iter;
				iter = adapter_interface.find(ip_forward_table_ptr->table[i].dwForwardIfIndex);
				router_info.interface_ = iter->second;
				router_info.metric_ = CPCodeChanger::UTF8ToUnicode(metric.c_str()).c_str();
				router_info.type_ = L"活动路由";
				all_router_information.push_back(router_info);
			}
		}
		if (ip_forward_table_ptr){
			free(ip_forward_table_ptr);
			ip_forward_table_ptr = NULL;
		}
		GetPermanentRoute(all_router_information);
		return true;
	}
	void RouterInformationHelper::GetAdaptersInformation(std::map<uint32, std::wstring>& interface_info)
	{
		PIP_ADAPTER_INFO adapter_info_ptr;
		PIP_ADAPTER_INFO adapter_ptr = NULL;
		DWORD result = 0;
		adapter_info_ptr = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
		ULONG buffer_len = sizeof(IP_ADAPTER_INFO);

		if (GetAdaptersInfo(adapter_info_ptr, &buffer_len) == ERROR_BUFFER_OVERFLOW){
			free(adapter_info_ptr);
			adapter_info_ptr = (IP_ADAPTER_INFO *)malloc(buffer_len);
		}

		if ((result = GetAdaptersInfo(adapter_info_ptr, &buffer_len)) == NO_ERROR)
		{
			adapter_ptr = adapter_info_ptr;

			while (adapter_ptr){
				char* IP = adapter_ptr->IpAddressList.IpAddress.String;
				std::wstring buffer = CPCodeChanger::UTF8ToUnicode(IP).c_str();
				interface_info.insert(make_pair(adapter_ptr->Index, buffer));
				adapter_ptr = adapter_ptr->Next;
			}
		}
		if (adapter_info_ptr){
			free(adapter_info_ptr);
			adapter_info_ptr = NULL;
		}
		interface_info[1] = std::wstring(L"127.0.0.1");
	}
	std::wstring RouterInformationHelper::ConvertDwordToIPString(uint32 ip)
	{
		std::wstring ip_string;
		in_addr ia;
		ia.S_un.S_addr = ip;
		char* pIP = inet_ntoa(ia);
		ip_string = CPCodeChanger::UTF8ToUnicode(pIP).c_str();
		return ip_string;
	}

	bool RouterInformationHelper::analyze(CPRecord* parentRecord)
	{
		AllRouterInformation  Router_info;
		GetRouterInformation(Router_info);
		if (Router_info.empty()) {
			return false;
		}

		for (auto it = Router_info.begin(); it != Router_info.end(); it++){
			RouterInformation& Router_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"NetWorkName") = Router_info.dest_address_;
			record->at(L"SubNetName") = Router_info.net_mask_;
			record->at(L"NetGate") = Router_info.gate_way_;
			record->at(L"Interface") = Router_info.interface_;
			record->at(L"phaseStep") = Router_info.metric_;
			record->at(L"Type") = Router_info.type_;
		}
		return true;
	}

	void RouterInformationHelper::GetPermanentRoute(AllRouterInformation& all_router_information)
	{
		HKEY RootKey;            // 主键
		LPCTSTR lpSubKey;        // 子键名称
		HKEY hkResult;            // 将要打开键的句柄 
		LONG lReturn;            // 记录读取注册表是否成功
		std::wstring wstrBuffer;
		DWORD index = 0;
		TCHAR szKeyName[255] = { 0 };        //名称
		DWORD dwKeyLen = 255;
		DWORD dwType = 0;
		RootKey = HKEY_LOCAL_MACHINE;
		lpSubKey = L"SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\PersistentRoutes";
		lReturn = RegOpenKeyEx(RootKey, lpSubKey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hkResult);

		if (lReturn == ERROR_SUCCESS){
			int nRet = 0;
			while (ERROR_SUCCESS == (nRet = RegEnumValue(hkResult, index, szKeyName, &dwKeyLen, NULL, &dwType, NULL, NULL))){
				index++;
				std::wstring wstrKeyName = szKeyName;
				std::vector<std::wstring>result;
				boost::split(result, wstrKeyName, boost::is_any_of(L","));
				RouterInformation router_info;
				if (result.size() == 4)   //IP、掩码、网关、跳数
				{
					router_info.dest_address_ = result[0];
					router_info.gate_way_ = result[2];
					router_info.net_mask_ = result[1];
					router_info.metric_ = result[3];
					router_info.type_ = L"永久路由";
				}
				all_router_information.push_back(router_info);
				dwKeyLen = 255;
				memset(szKeyName, 0, 255);
			}
			RegCloseKey(hkResult);
		}
	}
}
