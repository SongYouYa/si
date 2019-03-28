
#include "stdafx.h"
#include "NetShareSourceInfoHelper.h"
#include <Iphlpapi.h>
#include <LM.h>

namespace app
{
	bool  NetShareSourceInfoHelper::GetNetShareSourceInformation(NetShareSourcesInformation& share_sources_info)
	{
		PSHARE_INFO_502 buffer_ptr = NULL;
		PSHARE_INFO_502 p = NULL;
		NET_API_STATUS result;
		LPWSTR   server_ptr = NULL;
		DWORD entries_read = 0;
		DWORD total_entries = 0;
		DWORD resume_handle = 0;
		DWORD i;

		// DWORD er=0,tr=0,resume=0, i;
		do // begin do
		{
			result = NetShareEnum(server_ptr, 502, (LPBYTE *)&buffer_ptr, -1, &entries_read, &total_entries, &resume_handle);

			//
			// If the call succeeds,
			//
			if (result == ERROR_SUCCESS || result == ERROR_MORE_DATA)
			{
				p = buffer_ptr;
				//
				// Loop through the entries;
				//  print retrieved data.		
				for (i = 1; i <= entries_read; i++)
				{

					NetShareSourceInformation share_source_info;
					share_source_info.net_name_ = p->shi502_netname;
					share_source_info.path_ = p->shi502_path;
					share_source_info.password_ = p->shi502_passwd ? p->shi502_passwd : L"";
					share_source_info.security_descriptor_ = p->shi502_remark;
					wchar_t current_uses[32];
					swprintf_s(current_uses, 32, L"%d", p->shi502_current_uses);
					share_source_info.current_uses_ = current_uses;
					share_sources_info.push_back(share_source_info);
					p++;
				}

				//
				// Free the allocated buffer.
				//
				NetApiBufferFree(buffer_ptr);
			}

			//else
			//{
			//    printf("Error: %ld\n",result);
			//}
		}

		// Continue to call NetShareEnum while
		//  there are more entries.
		//
		while (result == ERROR_MORE_DATA); // end do

		return true;
	}

	bool NetShareSourceInfoHelper::analyze(CPRecord* parentRecord)
	{
		NetShareSourcesInformation  NetShareSources_info;
		GetNetShareSourceInformation(NetShareSources_info);
		if (NetShareSources_info.empty()) {
			return false;
		}

		for (auto it = NetShareSources_info.begin(); it != NetShareSources_info.end(); it++)
		{
			NetShareSourceInformation& NetShareSources_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"NetName") = NetShareSources_info.net_name_;
			record->at(L"Path") = NetShareSources_info.path_;
			record->at(L"CurrentUses") = NetShareSources_info.current_uses_;
			record->at(L"SecurityDiscriptor") = NetShareSources_info.security_descriptor_;
			record->at(L"Password") = NetShareSources_info.password_;
		}
		return true;
	}
}
