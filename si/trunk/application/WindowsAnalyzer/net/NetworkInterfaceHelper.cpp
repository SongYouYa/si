#include "stdafx.h"
#include <Iphlpapi.h>
#include <atltime.h>
#include "iptypes.h"
#include "sisystem\win\POsVersion.h"
#include "NetworkInterfaceHelper.h"
using namespace psystem;
// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//网络相关注册表位置：L"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces"
namespace app
{
    typedef struct _IP_ADAPTER_INFO_32 {
        struct _IP_ADAPTER_INFO* Next;
        DWORD ComboIndex;
        char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
        char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
        UINT AddressLength;
        BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
        DWORD Index;
        UINT Type;
        UINT DhcpEnabled;
        PIP_ADDR_STRING CurrentIpAddress;
        IP_ADDR_STRING IpAddressList;
        IP_ADDR_STRING GatewayList;
        IP_ADDR_STRING DhcpServer;
        BOOL HaveWins;
        IP_ADDR_STRING PrimaryWinsServer;
        IP_ADDR_STRING SecondaryWinsServer;
        DWORD LeaseObtained;
        DWORD LeaseExpires;
    } IP_ADAPTER_INFO_32, *PIP_ADAPTER_INFO_32;

    
    bool NetworkInterfaceHelper::ParseNetworkInfos(NetInterfaceInformation& net_interface_information)
    {
        ULONG ul_buf_len = sizeof(FIXED_INFO);
        FIXED_INFO* pFixedInfo = (FIXED_INFO *)MALLOC(ul_buf_len);
        
        if(ERROR_BUFFER_OVERFLOW == GetNetworkParams(pFixedInfo, &ul_buf_len))
        {
            FREE(pFixedInfo);
            pFixedInfo = (FIXED_INFO *)MALLOC(ul_buf_len);
        }
        
        if(ERROR_SUCCESS == GetNetworkParams(pFixedInfo, &ul_buf_len))
        {
            if (pFixedInfo->NodeType == BROADCAST_NODETYPE)
            {
                net_interface_information.node_type_ = L"广播"; //Broadcast";
            }
            else if(pFixedInfo->NodeType == PEER_TO_PEER_NODETYPE)
            {
                net_interface_information.node_type_ = L"P2P"; //"Peer To Peer";
            }
            else if(pFixedInfo->NodeType == MIXED_NODETYPE)
            {
                net_interface_information.node_type_ = L"混杂"; //Mixed";
            }
            else if(pFixedInfo->NodeType == HYBRID_NODETYPE)
            {
                net_interface_information.node_type_ = L"混合"; //"Hybrid"
            }
            else
            {
                net_interface_information.node_type_ = L"未知"; //"Unknown"
            }

			bool bWin2k = CPOsVersion::isBeforeVista();
            net_interface_information.net_config_name_ = bWin2k ? (L"Windows 2000 IP 配置") : (L"Windows IP 配置");
			std::string strHostName = pFixedInfo->HostName;
			net_interface_information.host_name_ =LS2W(strHostName.c_str());
            net_interface_information.primary_dns_suffix_ = L""; //没有找到来源
            net_interface_information.ip_route_enable_ = (pFixedInfo->EnableRouting) ? (L"是") : (L"否");
            //Specifies whether the local computer is acting as an ARP proxy.
            net_interface_information.wins_proxy_enable_ = (pFixedInfo->EnableProxy) ? (L"是") : (L"否");
        }
		else
		{
			FREE(pFixedInfo);
			return false;
		}
        
		FREE(pFixedInfo);
        return true;
    }

	bool NetworkInterfaceHelper::ParseAdapters(NetAdaptersInformation& infos)
	{
		/* Declare and initialize variables */
		DWORD dwSize = 0;
		DWORD dwRetVal = 0;

		LPVOID lpMsgBuf = NULL;

		PIP_ADAPTER_ADDRESSES pAddresses = NULL;
		ULONG outBufLen = 0;
		ULONG Iterations = 0;

		PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
		IP_ADAPTER_DNS_SERVER_ADDRESS *pDnServer = NULL;
		IP_ADAPTER_PREFIX *pPrefix = NULL;

		// Allocate a 15 KB buffer to start with.
		const ULONG WORKING_BUFFER_SIZE = 15000;
		const ULONG MAX_TRIES = 3;
		outBufLen = WORKING_BUFFER_SIZE;

		do {
			pAddresses = (IP_ADAPTER_ADDRESSES *) MALLOC(outBufLen);
			if (pAddresses == NULL) {
				return false;
			}

			dwRetVal =GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

			if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
				FREE(pAddresses);
				pAddresses = NULL;
			} else {
				break;
			}

			Iterations++;

		} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

		if (dwRetVal == NO_ERROR) {
			// If successful, output some information from the data we received
			pCurrAddresses = pAddresses;
			while (pCurrAddresses) {
				if (pCurrAddresses->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
				{
					pCurrAddresses = pCurrAddresses->Next;
					continue; //“环回适配器”不显示
				}

				NetAdapterInformation oneRecord;

				//连接名称
				oneRecord.friendname_ = GetAdapterTypeStr(pCurrAddresses->IfType);
				oneRecord.friendname_ += pCurrAddresses->FriendlyName;

				bool bRet = myGetPerAdapterInfo(pCurrAddresses->IfIndex, oneRecord.dns_server_, oneRecord.ip_autoconfiguration_);
				if (pCurrAddresses->IfType == IF_TYPE_TUNNEL)
				{
					oneRecord.ip_autoconfiguration_.clear(); //隧道适配器的自动配置获取不正确，这里清空
				}

				bRet = myGetIpAddrAndSubMask(pCurrAddresses->IfIndex, oneRecord.ip_address_, oneRecord.subnetmask_);
				bRet = myGetAdaptersInfo(pCurrAddresses->IfIndex, oneRecord.default_gateway_, oneRecord.dhcp_enable_, 
					oneRecord.dhcp_server_, oneRecord.lease_obtained_, oneRecord.lease_expires_);

				//在myGetPerAdapterInfo中获取的DnsServer为空，则填入ipv6默认的dns
				if (oneRecord.dns_server_.empty() && pCurrAddresses->Ipv6Enabled && 
					pCurrAddresses->IfType == IF_TYPE_ETHERNET_CSMACD) //以太网适配器
				{
					pDnServer = pCurrAddresses->FirstDnsServerAddress;
					for (DWORD i = 0; pDnServer != NULL; i++)
					{
						oneRecord.dns_server_ += L"fec0:0:0:ffff::" + boost::lexical_cast<std::wstring>(i+1) + L"%1  ";
						pDnServer = pDnServer->Next;
					}
				}

				oneRecord.connection_dns_suffix_ = pCurrAddresses->DnsSuffix;
				oneRecord.description_ = pCurrAddresses->Description; //描述

				//物理地址
				TCHAR phAdd[30] = {0};
				DWORD index = 0;
				if (pCurrAddresses->PhysicalAddressLength != 0) {
					for (DWORD i = 0; i < pCurrAddresses->PhysicalAddressLength; ++i) {
						if (index >= 30)
						{
							return false;
						}
						if (i == (pCurrAddresses->PhysicalAddressLength - 1))
							index += wsprintf(&phAdd[index], L"%.2X\n", (int) pCurrAddresses->PhysicalAddress[i]);
						else
							index += wsprintf(&phAdd[index], L"%.2X-", (int) pCurrAddresses->PhysicalAddress[i]);
					}
				}
				oneRecord.physical_address_ = phAdd;

				infos.push_back(oneRecord);

				pCurrAddresses = pCurrAddresses->Next;
			}
		}

		return true;
	}

	bool NetworkInterfaceHelper::myGetPerAdapterInfo(ULONG index, std::wstring& dnsServer, std::wstring& wstrIsAutoConfig)
	{
		PIP_PER_ADAPTER_INFO pPerAdapter = (IP_PER_ADAPTER_INFO*)malloc(sizeof(IP_INTERFACE_INFO));
		ULONG outBufLen = sizeof(IP_PER_ADAPTER_INFO);

		DWORD nRet = 0;
		nRet = GetPerAdapterInfo(index, pPerAdapter, &outBufLen);

		if (ERROR_BUFFER_OVERFLOW == nRet)
		{
			free(pPerAdapter);
			pPerAdapter = (IP_PER_ADAPTER_INFO*)malloc(outBufLen);
			pPerAdapter->DnsServerList.IpAddress.String[0] = '\0';
			nRet = GetPerAdapterInfo(index, pPerAdapter, &outBufLen);
			if (nRet != ERROR_SUCCESS)
			{
				return false;
			}
		}

		std::string strDnsServer = pPerAdapter->DnsServerList.IpAddress.String;
		dnsServer = LS2W(strDnsServer.c_str());
		wstrIsAutoConfig = pPerAdapter->AutoconfigEnabled ? L"是" :  L"否";

		free(pPerAdapter);
		return true;
	}

	bool NetworkInterfaceHelper::myGetIpAddrAndSubMask(DWORD ifIndex, std::wstring& wstrIpAddr, std::wstring& wstrSubNet)
	{
		DWORD dwSize = 0;
		PMIB_IPADDRTABLE pIPAddrTable;
		pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(sizeof (MIB_IPADDRTABLE));

		if (pIPAddrTable) {
			// Make an initial call to GetIpAddrTable to get the
			// necessary size into the dwSize variable
			if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) ==
				ERROR_INSUFFICIENT_BUFFER) {
					FREE(pIPAddrTable);
					pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(dwSize);
			}
		}
		else
		{
			return false;
		}

		DWORD dwRetVal = 0;

		// Make a second call to GetIpAddrTable to get the
		// actual data we want
		if ( (dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR ) { 
			FREE(pIPAddrTable);
			return false;
		}

		for (DWORD i=0; i < pIPAddrTable->dwNumEntries; i++) {
			MIB_IPADDRROW* table = &pIPAddrTable->table[i];
			if (ifIndex == table->dwIndex)
			{
				std::string strIpAddr;
				std::string strSubMask;
				IN_ADDR IPAddr;
				IPAddr.S_un.S_addr = (u_long) table->dwAddr;
				char* pIpAddr = inet_ntoa(IPAddr);
				if (pIpAddr)
				{
					strIpAddr = pIpAddr;
				}
				IPAddr.S_un.S_addr = (u_long) table->dwMask;
				char* pSubMask = inet_ntoa(IPAddr);
				if (pSubMask)
				{
					strSubMask = pSubMask;
				}

				wstrIpAddr = CPCodeChanger::GBToUnicode(strIpAddr.c_str());
				wstrSubNet = CPCodeChanger::GBToUnicode(strSubMask.c_str());
				return true;
			}
		}

		return false;
	}

	bool NetworkInterfaceHelper::myGetAdaptersInfo(DWORD ifIndex, std::wstring& wstrGetway, std::wstring& wstrDhcpEnable, 
		std::wstring& wstrDhcpServer,CPTime& leaseObtainedTime, CPTime& leaseExpireTime)
	{
		PIP_ADAPTER_INFO pAdapterInfo = NULL;
		DWORD dwRetVal = 0;

		DWORD outBuflen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (PIP_ADAPTER_INFO)MALLOC(outBuflen);
		if (pAdapterInfo == NULL)
		{
			return false;
		}

		if (GetAdaptersInfo(pAdapterInfo, &outBuflen) == ERROR_BUFFER_OVERFLOW)
		{
			FREE(pAdapterInfo);
			pAdapterInfo = (PIP_ADAPTER_INFO)MALLOC(outBuflen);
			if (pAdapterInfo == NULL)
			{
				return false;
			}
		}
		dwRetVal = GetAdaptersInfo(pAdapterInfo, &outBuflen);
		if (dwRetVal != NO_ERROR)
		{
			return false;
		}
		PIP_ADAPTER_INFO_32 pAdapter = (PIP_ADAPTER_INFO_32)pAdapterInfo;
		while(pAdapter)
		{
			if (ifIndex == pAdapter->Index)
			{
				std::string strGateway = pAdapter->GatewayList.IpAddress.String;
				wstrGetway = CPCodeChanger::GBToUnicode(strGateway.c_str());
				wstrDhcpEnable = pAdapter->DhcpEnabled ? L"是" : L"否";
				if (pAdapter->DhcpEnabled)
				{
					std::string strDhcpServer = pAdapter->DhcpServer.IpAddress.String;
					wstrDhcpServer = CPCodeChanger::GBToUnicode(strDhcpServer.c_str());
					leaseObtainedTime = CPTime::UTC2LocalTime((time_t)pAdapter->LeaseObtained, CPTime::GetCurrentTimeZoneBias());
					leaseExpireTime = CPTime::UTC2LocalTime((time_t)pAdapter->LeaseExpires, CPTime::GetCurrentTimeZoneBias());
				}
				return true;
			}
			else
				pAdapter = (PIP_ADAPTER_INFO_32)pAdapter->Next;
		}
		return false;
	}

	std::wstring NetworkInterfaceHelper::GetAdapterTypeStr( ULONG ifType )
	{
		std::wstring strIfType;
		switch(ifType)
		{
		case IF_TYPE_ETHERNET_CSMACD:
			strIfType = L"以太网";
			break;
		case IF_TYPE_SOFTWARE_LOOPBACK:
			strIfType = L"环回";
			break;
		case IF_TYPE_TUNNEL:
			strIfType = L"隧道";
			break;
		default:
			strIfType = L"未知";
			break;
		}
		strIfType += L"适配器 ";
		return strIfType;
	}

	bool NetworkInterfaceHelper::analyze(CPRecord* rootRecord)
	{
		NetInterfaceInformation  NetworkInterface_info;
		//Get network param information
		if (!ParseNetworkInfos(NetworkInterface_info))
		{
		}
	
		//Get network adapter information
		if (!ParseAdapters(NetworkInterface_info.adapters_info_))
		{
		}

		for (auto it = NetworkInterface_info.adapters_info_.begin(); it != NetworkInterface_info.adapters_info_.end(); ++it )
		{
			CPRecord* table = rootRecord->addChild();
			table->at(L"NetConfigName") = NetworkInterface_info.net_config_name_;
			table->at(L"HostName") = NetworkInterface_info.host_name_;
			table->at(L"PrimiaryDNSSuffix") = NetworkInterface_info.primary_dns_suffix_;;
			table->at(L"NodeType") = NetworkInterface_info.node_type_;
			table->at(L"IPRouteEnable") = NetworkInterface_info.ip_route_enable_;
			table->at(L"WinsProxyEnable") =NetworkInterface_info.wins_proxy_enable_;
			table->at(L"ConnecttionName") =it->friendname_;
			table->at(L"ConnectToSpecialDNSSuffix") =it->connection_dns_suffix_;
			table->at(L"Description") =it->description_;
			table->at(L"PhysicalAddress") =it->physical_address_;
			table->at(L"IsDHCP") =it->dhcp_enable_;
			table->at(L"IsAutoConfig") =it->ip_autoconfiguration_;
			table->at(L"IPAddr") =it->ip_address_;
			table->at(L"SubNet") =it->subnetmask_;
			table->at(L"NetGate") =it->default_gateway_;
			table->at(L"DHCPServer") =it->dhcp_server_;
			table->at(L"DNSServer") =it->dns_server_;
			table->at(L"LeaseObtained") = it->lease_obtained_;
			table->at(L"LeaseExpired") = it->lease_expires_;
		}

		return true;
	}

}
