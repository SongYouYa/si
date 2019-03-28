#include "stdafx.h"
#include "NetFilesInformationHelper.h"
#include <LM.h>

#pragma comment(lib, "Netapi32.lib")

namespace app
{
	bool NetFilesInformationHelper::GetNetFilesInformation(NetFilesInformation &net_files_information)
	{
		DWORD level, entries_read, total_entries;
		char* server_name;
		char* user_name;
		char* base_path;
		NET_API_STATUS status;
		FILE_INFO_3* buf_ptr = NULL;
		FILE_INFO_3* tmpbuf_ptr = NULL;
		server_name = user_name = base_path = NULL;
		entries_read = total_entries = status = 0;
		level = 3; 
		status = NetFileEnum(NULL, NULL, NULL,
			level,
			(LPBYTE*)&buf_ptr,
			MAX_PREFERRED_LENGTH,
			&entries_read,
			&total_entries,
			NULL);
		if (status != NERR_Success) {
			return false;
		}

		tmpbuf_ptr = buf_ptr;
		for (size_t i = 0; i < entries_read; ++i, ++tmpbuf_ptr)
		{
			NetFileInformation file_info;
			if ((tmpbuf_ptr->fi3_permissions & PERM_FILE_READ) == PERM_FILE_READ){
				file_info.access_attribute_ += L"Read";
			}
			if ((tmpbuf_ptr->fi3_permissions & PERM_FILE_WRITE) == PERM_FILE_WRITE){
				file_info.access_attribute_ += L"/Write";
			}
			if ((tmpbuf_ptr->fi3_permissions & PERM_FILE_CREATE) == PERM_FILE_CREATE)	{
				file_info.access_attribute_ += L"/Create";
			}
			if ((tmpbuf_ptr->fi3_permissions & ACCESS_EXEC) == ACCESS_EXEC){
				file_info.access_attribute_ += L"/Execute";
			}
			if ((tmpbuf_ptr->fi3_permissions & ACCESS_DELETE) == ACCESS_DELETE){
				file_info.access_attribute_ += L"/Delete";
			}
			file_info.lockes_ = tmpbuf_ptr->fi3_num_locks;
			file_info.user_name_ = (wchar_t*)tmpbuf_ptr->fi3_username;
			file_info.file_name_ = (wchar_t*)tmpbuf_ptr->fi3_pathname;
			net_files_information.push_back(file_info);
		}
		NetApiBufferFree(buf_ptr);
		buf_ptr = NULL;
		return true;
	}

	void NetFilesInformationHelper::EnumServer()
	{
		LPSERVER_INFO_100  pServerEnum;
		DWORD   dwResult, dwRead, dwTotal;
		dwResult = ::NetServerEnum(NULL, 100,
			(BYTE**)&pServerEnum,
			-1, &dwRead, &dwTotal,
			SV_TYPE_ALL, NULL, 0);
		if (dwResult == NERR_Success){
			for (uint i = 0; i < dwTotal; i++)
			LPCWSTR lpchar = (LPCWSTR)pServerEnum[i].sv100_name;
		}
	}

	bool NetFilesInformationHelper::analyze(CPRecord* parentRecord)
	{
		NetFilesInformation  NetFile_info;
		if (!GetNetFilesInformation(NetFile_info)) {
			return false;
		}
		for (auto it = NetFile_info.begin(); it != NetFile_info.end(); it++){
			NetFileInformation& NetFile_info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"FileName") = NetFile_info.file_name_;
			record->at(L"UserName") = NetFile_info.user_name_;
			record->at(L"Lockes") = NetFile_info.lockes_;
			record->at(L"AccessAttribute") = NetFile_info.access_attribute_;
		}
		return true;
	}
}
