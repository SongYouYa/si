#include "stdafx.h"
#include "PromiscPortHelper.h"
#include <Iphlpapi.h>
#include <ntddndis.h>

#pragma comment(lib,"iphlpapi.lib")

namespace app
{
	bool  PromiscPortHelper::GetPromiscPortInformation(PromiscPortInformation& promisc_port_information)
	{
		ShowAdaptersInfo();
		PIP_ADAPTER_INFO pAadapterInfo;
		pAadapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
		ULONG lSize = sizeof(IP_ADAPTER_INFO);

		if (ERROR_BUFFER_OVERFLOW == GetAdaptersInfo(pAadapterInfo, &lSize))
		{
			free(pAadapterInfo);
			pAadapterInfo = (IP_ADAPTER_INFO *)malloc(lSize);
		}

		if (NO_ERROR == GetAdaptersInfo(pAadapterInfo, &lSize))
		{
			PIP_ADAPTER_INFO pAdapter = pAadapterInfo;

			while (NULL != pAdapter)
			{
				char szDeviceName[MAX_ADAPTER_NAME_LENGTH + 4] = { 0 };
				strcat_s(szDeviceName, "\\\\.\\");
				strcat_s(szDeviceName, pAdapter->AdapterName);
				HANDLE hFile = CreateFileA(szDeviceName,
					GENERIC_READ,
					FILE_SHARE_READ,
					NULL,
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL,
					INVALID_HANDLE_VALUE);

				if (INVALID_HANDLE_VALUE != hFile)
				{
					DWORD dwInBuf = OID_GEN_CURRENT_PACKET_FILTER;
					DWORD dwOutBuf = 0;
					DWORD dwReturn = 0;
					if (DeviceIoControl(hFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwInBuf, sizeof(dwInBuf), &dwOutBuf, sizeof(dwOutBuf), &dwReturn, NULL))
					{
						if ((dwOutBuf & 0x20) != 0)
						{
							wchar_t* _wstr = new wchar_t[strlen(pAdapter->Description) + 1];
							MultiByteToWideChar(CP_ACP, MB_COMPOSITE, pAdapter->Description, -1, _wstr, (int)strlen(pAdapter->Description) + 1);
							std::wstring adapter_description = _wstr;
							promisc_port_information.adapters_.push_back(adapter_description);
							delete[] _wstr;
						}
					}

					CloseHandle(hFile);
				}

				pAdapter = pAdapter->Next;
			};
		}

		free(pAadapterInfo);
		return true;
	}

	void PromiscPortHelper::ShowAdaptersInfo()
	{
		PIP_ADAPTER_INFO pAdapterInfo;
		PIP_ADAPTER_INFO pAdapter = NULL;
		DWORD errValue = 0;
		ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
		if (!pAdapterInfo)//malloc失败
			return;
		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) //空间不够，重新分配
		{
			free(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO *)malloc(ulOutBufLen);
			if (!pAdapterInfo) //malloc失败
				return;
		}
	
		if ((errValue = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
		{
			
		}
		if (pAdapterInfo)
			free(pAdapterInfo);
	}

	bool PromiscPortHelper::analyze(CPRecord *parentRecord)
	{
		PromiscPortInformation  PromiscPort_info;
		GetPromiscPortInformation(PromiscPort_info);

		for (auto &adapter : PromiscPort_info.adapters_){
			CPRecord* record = parentRecord->addChild();
			record->at(L"Adapters") = adapter;
		}	
		return true;
	}
}
