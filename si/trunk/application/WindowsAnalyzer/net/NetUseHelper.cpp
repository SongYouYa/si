#include "stdafx.h"
#include "NetUseHelper.h"
#include <Winnetwk.h>

#pragma comment(lib, "Mpr.lib")

namespace app
{
	NetUseHelper::NetUseHelper()
	{
	}
	NetUseHelper::~NetUseHelper()
	{
	}
	bool  NetUseHelper::GetNetUseInformation(NetUseInformation& net_use_information)
	{
		//2011 12 10 17:35 modify by wujianghuang
		HANDLE hEnum = NULL;
		//    WNetOpenEnum(RESOURCE_REMEMBERED, RESOURCETYPE_DISK, 0, NULL, &hEnum);
		if (WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_DISK,
			0, NULL, &hEnum) != NO_ERROR){
			return false;
		}

		DWORD count = -1;// 0xffffffff;
		DWORD buf_size = 16384;// sizeof(NETRESOURCE) * 10;
		LPNETRESOURCE buffer_ptr = (LPNETRESOURCE)GlobalAlloc(GPTR, buf_size);
		DWORD result;

		do
		{
			ZeroMemory(buffer_ptr, buf_size);
			result = WNetEnumResource(hEnum, &count, buffer_ptr, &buf_size);
			if (result == NO_ERROR)
			{
				for (size_t i = 0; i < count && i < buf_size / sizeof(NETRESOURCE); ++i)
				{
					NetUseRecord  netuse_record;
					// HANDLE h;
					DWORD r;
					//  r = WNetOpenEnum(RESOURCE_GLOBALNET, RESOURCETYPE_DISK, 0, &buffer_ptr[i], &h);
					wchar_t remotename[1024] = L"";
					DWORD namelength = 1024;
					r = WNetGetConnectionW(buffer_ptr[i].lpLocalName, remotename, &namelength);

					if (r == ERROR_SUCCESS)
					{
						netuse_record.status_ = L"可用";
					}
					else
					{
						netuse_record.status_ = L"不可用";
					}

					netuse_record.local_ = buffer_ptr[i].lpLocalName == NULL ? L"" : buffer_ptr[i].lpLocalName;//[i].lpRemoteName;
					netuse_record.remote_ = buffer_ptr[i].lpRemoteName;
					netuse_record.net_type_ = buffer_ptr[i].lpProvider;
					net_use_information.push_back(netuse_record);

					//  WNetCloseEnum(h);
				}
			}
			else if (result != ERROR_NO_MORE_ITEMS)
			{
				break;
			}
		} while (result != ERROR_NO_MORE_ITEMS);

		GlobalFree((HGLOBAL)buffer_ptr);
		WNetCloseEnum(hEnum);
		return true;
	}

	bool NetUseHelper::analyze(CPRecord* parentRecord)
	{
		NetUseInformation  NetUse_info;
		GetNetUseInformation(NetUse_info);
		if (NetUse_info.empty()) {
			return false;
		}

		for (auto it = NetUse_info.begin(); it != NetUse_info.end(); it++)
		{
			NetUseRecord& NetUse_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"Status") = NetUse_info.status_;
			record->at(L"LocalIP") = NetUse_info.local_;
			record->at(L"RemoteIP") = NetUse_info.remote_;
			record->at(L"ConnecttionType") = NetUse_info.net_type_;
		}
		return true;
	}

}
