#include "stdafx.h"
#include "PortInformationHelper.h"
#include <Tlhelp32.h>
#include <Psapi.h>
#pragma comment(lib, "ws2_32.lib")
#define HOSTNAMELEN 256
#define PORTNAMELEN 256
#define ADDRESSLEN HOSTNAMELEN+PORTNAMELEN

namespace app
{
	// TCP ×´Ì¬
	WCHAR TcpState[][32] = { L"", L"CLOSED", L"LISTENING", L"SYN_SENT", L"SYN_RCVD",
		L"ESTABLISHED", L"FIN_WAIT1", L"FIN_WAIT2", L"CLOSE_WAIT",
		L"CLOSING", L"LAST_ACK", L"TIME_WAIT", L"DELETE_TCB"
	};
	PortInformationHelper::PortInformationHelper()
	{
		hDLL = NULL;
	}
	PortInformationHelper::~PortInformationHelper()
	{
	}
	bool PortInformationHelper::LoadAPIOfWindowsXPOrEarlier()
	{
		hDLL = LoadLibrary(L"iphlpapi.dll");

		if (hDLL != NULL)
		{
			allocate_and_gettcpextable_from_stack_ptr_ =
				(PALLOCATE_AND_GET_TCPEXTABLE_FROM_STACK)(GetProcAddress(hDLL, "AllocateAndGetTcpExTableFromStack"));
			allocate_and_getudpextable_from_stack_ptr_ =
				(PALLOCATE_AND_GET_UDPEXTABLE_FROM_STACK)(GetProcAddress(hDLL, "AllocateAndGetUdpExTableFromStack"));

			if (allocate_and_gettcpextable_from_stack_ptr_ != NULL &&
				allocate_and_getudpextable_from_stack_ptr_ != NULL)
			{
				return TRUE;
			}
			else{
				FreeLibrary(hDLL);
				hDLL = NULL;
			}
		}

		return FALSE;
	}
	bool PortInformationHelper::LoadAPIOfWindowsVistaOrLater()
	{
		hDLL = LoadLibrary(L"iphlpapi.dll");

		if (hDLL != NULL){
			get_extended_tcp_table_ptr_ = (PGET_EXTENDED_TCP_TABLE)(GetProcAddress(hDLL, "GetExtendedTcpTable"));
			get_extended_udp_table_ptr_ = (PGET_EXTENDED_UDP_TABLE)(GetProcAddress(hDLL, "GetExtendedUdpTable"));

			if (get_extended_tcp_table_ptr_ != NULL && get_extended_udp_table_ptr_ != NULL){
				return TRUE;
			}
			else{
				FreeLibrary(hDLL);
				hDLL = NULL;
			}
		}
		return FALSE;
	}
	void PortInformationHelper::FreeAPI(void)
	{
		if (hDLL != NULL){
			FreeLibrary(hDLL);
			hDLL = NULL;
		}
	}
	bool PortInformationHelper::Initial()
	{
		WSADATA WSAData;
		WORD wVersion;
		int ret = 0;
		wVersion = MAKEWORD(1, 1);
		ret = WSAStartup(MAKEWORD(1, 1), &WSAData);

		if (ret != 0)
		{
			return FALSE;
		}
		if (!psystem::CPOsVersion::isVistaOrLater())
		{
			if (!LoadAPIOfWindowsXPOrEarlier()) return FALSE;
		}
		else
		{
			if (!LoadAPIOfWindowsVistaOrLater()) return FALSE;
		}
		return TRUE;
	}
	bool  PortInformationHelper::GetTCPPortInformation(PortInformation& tcp_port_information)
	{
		if (psystem::CPOsVersion::isVistaOrLater())
		{
			if (GetTCPPortInformationByLaterAPI(tcp_port_information))
			{
				return true;
			}
		}
		else
		{
			if (GetTCPPortInformationByEarlierAPI(tcp_port_information))
			{
				return true;
			}
		}
		return false;
	}
	bool PortInformationHelper::GetUDPPortInformation(PortInformation& udp_port_information)
	{
		if (psystem::CPOsVersion::isVistaOrLater())
		{
			if (GetUDPPortInformationByLaterAPI(udp_port_information))
			{
				return true;
			}
		}
		else
		{
			if (GetUDPPortInformationByEarlierAPI(udp_port_information))
			{
				return true;
			}
		}
		return false;
	}
	bool PortInformationHelper::GetTCPPortInformationByEarlierAPI(PortInformation& tcp_port_info)
	{
		WCHAR local_address[ADDRESSLEN];
		WCHAR remote_address[ADDRESSLEN];
		if (NULL == allocate_and_gettcpextable_from_stack_ptr_) return false;;
		int nRetCode = allocate_and_gettcpextable_from_stack_ptr_(&tcpex_table_ptr_, TRUE, GetProcessHeap(), 2, 2);
		if (nRetCode){
			return false;
		}
		for (size_t i = 0; i < tcpex_table_ptr_->dwNumEntries; i++)
		{
			swprintf_s(local_address, ADDRESSLEN, L"%s:%s",
				GetIP(tcpex_table_ptr_->table[i].dwLocalAddr).c_str(),
				GetPort(tcpex_table_ptr_->table[i].dwLocalPort).c_str()
				);
			swprintf_s(remote_address, ADDRESSLEN, L"%s:%s",
				GetIP(tcpex_table_ptr_->table[i].dwRemoteAddr).c_str(),
				GetPort(tcpex_table_ptr_->table[i].dwRemotePort).c_str()
				);
			PortRecord tcp_port_record;
			tcp_port_record.process_name_ = GetProcessNameFromPID(tcpex_table_ptr_->table[i].dwProcessId);
			tcp_port_record.process_id_ = tcpex_table_ptr_->table[i].dwProcessId;
			tcp_port_record.protocol_ = L"TCP";
			tcp_port_record.local_address_ = local_address;
			tcp_port_record.foreign_address_ = remote_address;

			// ÐÞÏÂÒ»´¦ bug
			if (tcpex_table_ptr_->table[i].dwState >= sizeof(TcpState) / sizeof(*TcpState))
				tcp_port_record.state_ = TcpState[0];
			else
				tcp_port_record.state_ = TcpState[tcpex_table_ptr_->table[i].dwState];

			tcp_port_record.process_path_ = GetProcessPathFromPID(tcpex_table_ptr_->table[i].dwProcessId);
			tcp_port_info.push_back(tcp_port_record);
		}
		return true;
	}
	bool PortInformationHelper::GetTCPPortInformationByLaterAPI(PortInformation& tcp_port_info)
	{
		WCHAR local_address[ADDRESSLEN];  // HOSTNAMELEN+PORTNAMELEN = 256+256;
		memset(local_address, 0, ADDRESSLEN);
		WCHAR remote_address[ADDRESSLEN];
		memset(remote_address, 0, ADDRESSLEN);
		PMIB_TCPTABLE_OWNER_PID tcp_table_ptr = NULL;
		DWORD dwSize = 0;
		if (NULL == *get_extended_tcp_table_ptr_) return false;
		if (get_extended_tcp_table_ptr_(tcp_table_ptr, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0) == ERROR_INSUFFICIENT_BUFFER)
		{
			GlobalFree(tcp_table_ptr);
			tcp_table_ptr = (PMIB_TCPTABLE_OWNER_PID)(malloc((UINT)dwSize));
		}
		if ((get_extended_tcp_table_ptr_(tcp_table_ptr, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) == NO_ERROR)
		{
			for (size_t i = 0; i < tcp_table_ptr->dwNumEntries; i++)
			{
				swprintf_s(local_address, 512, L"%s:%s",
					GetIP(tcp_table_ptr->table[i].dwLocalAddr).c_str(),
					GetPort(tcp_table_ptr->table[i].dwLocalPort).c_str()
					);
				swprintf_s(remote_address, 512, L"%s:%s",
					GetIP(tcp_table_ptr->table[i].dwRemoteAddr).c_str(),
					GetPort(tcp_table_ptr->table[i].dwRemotePort).c_str()
					);
				/*WCHAR process_id[_MAX_PATH];
				swprintf_s(process_id, _MAX_PATH, L"%d", tcp_table_ptr->table[i].dwOwningPid);*/
				PortRecord tcp_record;
				tcp_record.process_name_ = GetProcessNameFromPID(tcp_table_ptr->table[i].dwOwningPid);
				tcp_record.process_path_ = GetProcessPathFromPID(tcp_table_ptr->table[i].dwOwningPid);
				tcp_record.process_id_ = tcp_table_ptr->table[i].dwOwningPid;
				tcp_record.protocol_ = L"TCP";
				tcp_record.local_address_ = local_address;
				tcp_record.foreign_address_ = remote_address;

				if (tcp_table_ptr->table[i].dwState >= 12)
				{
					tcp_record.state_ = GetTcpState(0);
				}
				else
				{
					tcp_record.state_ = GetTcpState(tcp_table_ptr->table[i].dwState);
				}

				tcp_port_info.push_back(tcp_record);
			}
		}
		if (tcp_table_ptr)
		{
			free(tcp_table_ptr);
			tcp_table_ptr = NULL;
		}
		return true;
	}
	bool PortInformationHelper::GetUDPPortInformationByLaterAPI(PortInformation& udp_port_info)
	{
		WCHAR local_address[ADDRESSLEN];
		PMIB_UDPTABLE_OWNER_PID udp_table_ptr = NULL;
		DWORD dwSize = 0;
		if (NULL == get_extended_udp_table_ptr_) return false;
		if (get_extended_udp_table_ptr_(udp_table_ptr, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0) == ERROR_INSUFFICIENT_BUFFER)
		{
			GlobalFree(udp_table_ptr);
			udp_table_ptr = (PMIB_UDPTABLE_OWNER_PID)(malloc((UINT)dwSize));
		}

		if ((get_extended_udp_table_ptr_(udp_table_ptr, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) == NO_ERROR)
		{
			for (size_t i = 0; i < udp_table_ptr->dwNumEntries; i++)
			{
				swprintf_s(local_address, ADDRESSLEN, L"%s:%s",
					GetIP(udp_table_ptr->table[i].dwLocalAddr).c_str(),
					GetPort(udp_table_ptr->table[i].dwLocalPort).c_str()
					);
				PortRecord udp_port_record;
				udp_port_record.process_name_ = GetProcessNameFromPID(udp_table_ptr->table[i].dwOwningPid);
				udp_port_record.process_path_ = GetProcessPathFromPID(udp_table_ptr->table[i].dwOwningPid);
				udp_port_record.process_id_ = udp_table_ptr->table[i].dwOwningPid;
				udp_port_record.protocol_ = L"UDP";
				udp_port_record.local_address_ = local_address;
				udp_port_record.foreign_address_ = std::wstring(L"*:*");
				udp_port_info.push_back(udp_port_record);
			}
		}
		else
		{
			return false;
		}

		if (udp_table_ptr)
		{
			free(udp_table_ptr);
			udp_table_ptr = NULL;
		}
		return true;
	}
	bool PortInformationHelper::GetUDPPortInformationByEarlierAPI(PortInformation& udp_port_info)
	{
		WCHAR local_address[ADDRESSLEN];

		if (NULL == allocate_and_getudpextable_from_stack_ptr_) return false;

		int nRetCode = allocate_and_getudpextable_from_stack_ptr_(&udpex_table_ptr_, TRUE, GetProcessHeap(), 2, 2);

		if (nRetCode)
		{
			return  false;
		}
		for (size_t i = 0; i < udpex_table_ptr_->dwNumEntries; i++)
		{
			swprintf_s(local_address, ADDRESSLEN, L"%s:%s",
				GetIP(tcpex_table_ptr_->table[i].dwLocalAddr).c_str(),
				GetPort(tcpex_table_ptr_->table[i].dwLocalPort).c_str()
				);
			//WCHAR process_id[_MAX_PATH];
			//swprintf_s(process_id, _MAX_PATH, L"%d", tcpex_table_ptr_->table[i].dwProcessId);
			PortRecord udp_port_record;
			udp_port_record.process_name_ = GetProcessNameFromPID(tcpex_table_ptr_->table[i].dwProcessId);
			udp_port_record.process_id_ = tcpex_table_ptr_->table[i].dwProcessId;
			udp_port_record.protocol_ = L"UDP";
			udp_port_record.local_address_ = local_address;
			udp_port_record.foreign_address_ = std::wstring(L"*:*");
			udp_port_record.process_path_ = GetProcessPathFromPID(tcpex_table_ptr_->table[i].dwProcessId);
			udp_port_info.push_back(udp_port_record);
		}

		return false;
	}
	std::wstring PortInformationHelper::GetProcessNameFromPID(DWORD processID)
	{
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;
		// Take a snapshot of all processes in the app.
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap == INVALID_HANDLE_VALUE){
			return std::wstring();
		}
		// Set the size of the structure before using it.
		pe32.dwSize = sizeof(PROCESSENTRY32);

		// Retrieve information about the first process,
		// and exit if unsuccessful
		if (!Process32First(hProcessSnap, &pe32))
		{
			CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
			return std::wstring();
		}

		do
		{
			if (pe32.th32ProcessID == processID)
			{
				CloseHandle(hProcessSnap);
				TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
				// Get a handle to the process.
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
					PROCESS_VM_READ,
					FALSE, processID);
				return pe32.szExeFile;
			}
		} while (Process32Next(hProcessSnap, &pe32));

		return std::wstring();
	}
	std::wstring PortInformationHelper::GetProcessPathFromPID(DWORD processID)
	{
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;
		// Take a snapshot of all processes in the app.
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			return std::wstring();
		}
		// Set the size of the structure before using it.
		pe32.dwSize = sizeof(PROCESSENTRY32);
		// Retrieve information about the first process,
		// and exit if unsuccessful
		if (!Process32First(hProcessSnap, &pe32)){
			CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
			return std::wstring();
		}
		do
		{
			if (pe32.th32ProcessID == processID)
			{
				CloseHandle(hProcessSnap);
				WCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
				// Get a handle to the process.
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,
					FALSE, processID);
				WCHAR szFileName[MAX_PATH] = L"\0";
				GetProcessImageFileName(hProcess, szFileName, sizeof(szFileName));
				return szFileName;
			}
		} while (Process32Next(hProcessSnap, &pe32));

		return std::wstring();
	}
	std::wstring PortInformationHelper::GetPort(unsigned int  port)
	{
		WCHAR port_str[256] = L"\0";
		swprintf_s(port_str, 256, L"%d", htons((WORD)port));
		//return szPort;
		// std::wstring port_str = boost::lexical_cast<std::wstring>(htons(port));
		return port_str;
	}
	std::wstring PortInformationHelper::GetIP(unsigned int ipaddr)
	{
		DWORD ip_addr;
		WCHAR ip_str[256] = L"\0";
		ip_addr = htonl(ipaddr);
		swprintf_s(ip_str, 256, L"%d.%d.%d.%d", (ip_addr >> 24) & 0xFF,
			(ip_addr >> 16) & 0xFF, (ip_addr >> 8) & 0xFF,
			(ip_addr)& 0xFF);
		return ip_str;
	}
	std::wstring PortInformationHelper::GetTcpState(DWORD state)
	{
		std::wstring tcp_state;

		switch (state)
		{
		case 0:
			tcp_state = L"";
			break;

		case 1:
			tcp_state = L"CLOSED";
			break;

		case 2:
			tcp_state = L"LISTENING";
			break;

		case 3:
			tcp_state = L"SYN_SENT";
			break;

		case 4:
			tcp_state = L"SYN_RCVD";
			break;

		case 5:
			tcp_state = L"ESTABLISHED";
			break;

		case 6:
			tcp_state = L"FIN_WAIT1";
			break;

		case 7:
			tcp_state = L"FIN_WAIT2";
			break;

		case 8:
			tcp_state = L"CLOSE_WAIT";
			break;

		case 9:
			tcp_state = L"CLOSING";
			break;

		case 10:
			tcp_state = L"LAST_ACK";
			break;

		case 11:
			tcp_state = L"TIME_WAIT";
			break;

		case 12:
			tcp_state = L"DELETE_TCB";
			break;

		default:
			tcp_state = L"";
			break;
		}

		return tcp_state;
	}

	bool PortInformationHelper::analyze(CPRecord* parentRecord)
	{
		PortInformation all_info;
		Initial();
		if (GetTCPPortInformation(all_info) || GetUDPPortInformation(all_info))
		{
			for (auto it = all_info.begin(); it != all_info.end(); ++it)
			{
				PortRecord& NetShareSources_info = *it;
				CPRecord* record = parentRecord->addChild();
				record->at(L"protocol") = NetShareSources_info.protocol_;
				record->at(L"ProcessName") = NetShareSources_info.process_name_;
				record->at(L"ProcessID") = NetShareSources_info.process_id_;
				record->at(L"LocalIP") = NetShareSources_info.local_address_;
				record->at(L"RemoteIP") = NetShareSources_info.foreign_address_;
				record->at(L"Status") = NetShareSources_info.state_;
				record->at(L"Path") = NetShareSources_info.process_path_;
			}
			return true;
		}
		return true;
	}
}