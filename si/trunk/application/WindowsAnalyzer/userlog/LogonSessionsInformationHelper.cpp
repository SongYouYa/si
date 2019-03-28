#include "stdafx.h"
#include "LogonSessionsInformationHelper.h"

#include <NTSecAPI.h>
#include <Sddl.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

namespace app
{
	typedef NTSTATUS(NTAPI *PLsaEnumerateLogonSessions)(PULONG LogonSessionCount, PLUID* LogonSessionList);
	typedef NTSTATUS(NTAPI *PLsaFreeReturnBuffer)(PVOID Buffer);
	typedef NTSTATUS(NTAPI *PLsaGetLogonSessionData)(PLUID LogonId, PSECURITY_LOGON_SESSION_DATA* ppLogonSessionData);
	bool LogonSessionsInformationHelper::GetLogonSessionsInformation(LogonSessionsInformation& logon_sessions_information)
	{
		std::vector<uint32> process_ids;
		if (!GetAllProcessIds(process_ids)){
			return false;
		}
		std::vector<std::wstring> process_paths;
		std::vector<PTOKEN_USER> users;
		GetAllProcesspaths(process_ids, process_paths, users);
		bool nret = GetLogonSessionInfoByLoadLibrary(users, process_ids, process_paths, logon_sessions_information);
		if (!nret){
			return false;
		}
		else{
			return true;
		}
	}
	//--------------------------------------get all processes' id
	bool LogonSessionsInformationHelper::GetAllProcessIds(std::vector<uint32>& process_ids)
	{
		DWORD * ids_ptr = new DWORD[1024];
		DWORD count = 1024;
		DWORD bytes_returned = 0;
		bool  success = false;

		if (!EnumProcesses(ids_ptr, count, &bytes_returned))
		{
			delete[]ids_ptr;
			ids_ptr = NULL;
			ids_ptr = new DWORD[bytes_returned / sizeof(DWORD)];
			count = bytes_returned / sizeof(DWORD);

			if (EnumProcesses(ids_ptr, count, &bytes_returned))
			{
				success = true;
			}
		}
		else
		{
			success = true;
		}

		if (success)
		{
			for (DWORD i = 0; i < bytes_returned / sizeof(DWORD); ++i)
			{
				process_ids.push_back(ids_ptr[i]);
			}
		}

		delete[] ids_ptr;
		ids_ptr = NULL;
		return success;
	}

	bool LogonSessionsInformationHelper::GetAllProcesspaths(const std::vector<uint32>& process_ids,
		std::vector<std::wstring>& process_paths, std::vector<PTOKEN_USER>& users)
	{ //通過进程id 进行罗列进程信息
		for (std::vector<uint32>::const_iterator i = process_ids.begin(); i != process_ids.end(); ++i)
		{
			PTOKEN_USER ptuser = NULL;
			std::wstring path = L"";
			WCHAR wpath[MAX_PATH] = L"";
			HANDLE hprocess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, *i);

			if (hprocess != NULL)
			{
				//get path
				if (GetProcessImageFileNameW(hprocess, wpath, MAX_PATH))
				{
					path = wpath;
				}
			}
			else
			{
				hprocess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, *i);

				if (hprocess != NULL)
				{
					//反回一份执行进程文件的路径
					if (GetModuleFileNameExW(hprocess, NULL, wpath, MAX_PATH))
					{
						path = wpath;
					}
				}
			}

			if (hprocess != NULL)
			{
				HANDLE htoken = NULL;

				if (OpenProcessToken(hprocess, TOKEN_QUERY, &htoken))
				{
					DWORD lenReturn = 0;

					if (!GetTokenInformation(htoken, TokenUser, NULL, 0, &lenReturn))
					{
						ptuser = (PTOKEN_USER)new BYTE[lenReturn];

						if (!GetTokenInformation(htoken, TokenUser, ptuser, lenReturn, &lenReturn))
						{
							delete[] ptuser;
							ptuser = NULL;
						}
					}

					CloseHandle(htoken);
				}

				CloseHandle(hprocess);
			}

			users.push_back(ptuser);
			process_paths.push_back(path);
		}

		return true;
	}
	//------------------------------------- get other attributes excludes id,path by load library "secur32.dll"
	bool LogonSessionsInformationHelper::GetLogonSessionInfoByLoadLibrary(
		const std::vector<PTOKEN_USER>& users,
		const std::vector<uint32>& process_ids,
		const std::vector<std::wstring>& process_paths,
		LogonSessionsInformation& logon_sessions_information
		)
	{
		HMODULE hLib = ::LoadLibrary(L"secur32.dll");

		if (hLib != NULL)
		{
			PLsaEnumerateLogonSessions pLsaEnumerateLogonSessions = (PLsaEnumerateLogonSessions)GetProcAddress(hLib, "LsaEnumerateLogonSessions");
			PLsaGetLogonSessionData pLsaGetLogonSessionData = (PLsaGetLogonSessionData)GetProcAddress(hLib, "LsaGetLogonSessionData");
			PLsaFreeReturnBuffer pLsaFreeReturnBuffer = (PLsaFreeReturnBuffer)GetProcAddress(hLib, "LsaFreeReturnBuffer");

			if (pLsaEnumerateLogonSessions != NULL && pLsaGetLogonSessionData != NULL && pLsaFreeReturnBuffer != NULL)
			{
				ULONG logsessionCounts;
				PLUID plogsessionList = NULL;

				if (pLsaEnumerateLogonSessions(&logsessionCounts, &plogsessionList) == ((NTSTATUS)0x00000000L) && plogsessionList != NULL)
				{
					for (ULONG i = 0; i < logsessionCounts; ++i)
					{
						PSECURITY_LOGON_SESSION_DATA pLogonSessionData = NULL;

						if (pLsaGetLogonSessionData(plogsessionList + i, &pLogonSessionData) == ((NTSTATUS)0x00000000L))
						{
							LogonSessionInformation logon_session_info;
							WCHAR tmp_string[MAX_PATH];
							swprintf(tmp_string, 100, L"%08lx:%08lx", pLogonSessionData->LogonId.HighPart, pLogonSessionData->LogonId.LowPart);
							logon_session_info.logon_session_ = tmp_string;

							if (pLogonSessionData->UserName.Length != 0 && pLogonSessionData->UserName.Buffer != NULL)
							{
								logon_session_info.user_name_ = pLogonSessionData->UserName.Buffer;
							}

							if (pLogonSessionData->AuthenticationPackage.Length != 0 && pLogonSessionData->AuthenticationPackage.Buffer != NULL)
							{
								logon_session_info.auth_package_ = pLogonSessionData->AuthenticationPackage.Buffer;
							}

							std::wstring logonType;

							switch (pLogonSessionData->LogonType)
							{
							case Interactive:
								logonType = L"Interactive";
								break;

							case Network:
								logonType = L"Network";
								break;

							case Service:
								logonType = L"Service";
								break;

							case Proxy:
								logonType = L"Proxy";
								break;

							case Unlock:
								logonType = L"Unlock";
								break;

							default:
								break;
							}

							logon_session_info.logon_type_ = logonType;
							logon_session_info.session_ = pLogonSessionData->Session;
							LPWSTR sid = NULL;

							if (IsValidSid(pLogonSessionData->Sid))
							{
								ConvertSidToStringSidW(pLogonSessionData->Sid, &sid);
							}

							if (sid != NULL)
							{
								logon_session_info.sid_ = sid;
								LocalFree(sid);
							}

							int64 timet = TimeConverter::LargeIntegerToTimet(pLogonSessionData->LogonTime);
							logon_session_info.logon_time_ = CPTime::UTC2LocalTime(timet, CPTime::GetCurrentTimeZoneBias());
							//獲取失敗
							if (!psystem::CPOsVersion::isBeforeVista())
							{
								if (pLogonSessionData->LogonServer.Length != 0 && pLogonSessionData->LogonServer.Buffer != NULL)
								{
									logon_session_info.logon_server_ = pLogonSessionData->LogonServer.Buffer;
								}

								if (pLogonSessionData->DnsDomainName.Length != 0 && pLogonSessionData->DnsDomainName.Buffer != NULL)
								{
									logon_session_info.dns_domain_ = pLogonSessionData->DnsDomainName.Buffer;
								}

								if (pLogonSessionData->Upn.Length != 0 && pLogonSessionData->Upn.Buffer != NULL)
								{
									logon_session_info.upn_ = pLogonSessionData->Upn.Buffer;
								}
							}

							for (size_t j = 0; j < users.size(); ++j)
							{
								PTOKEN_USER user_ptoken = users[j];

								if (user_ptoken != NULL && IsValidSid(user_ptoken->User.Sid)
									&& IsValidSid(pLogonSessionData->Sid)
									&& EqualSid(user_ptoken->User.Sid, pLogonSessionData->Sid))
								{
									logon_session_info.processlist_[process_ids[j]] = process_paths[j];
								}
							}


							logon_sessions_information.push_back(logon_session_info);
						}

						if (pLogonSessionData != NULL)
						{
							pLsaFreeReturnBuffer(pLogonSessionData);
						}
					}
				}

				if (plogsessionList != NULL)
				{
					pLsaFreeReturnBuffer(plogsessionList);
				}
			}
		}

		for (size_t i = 0; i < users.size(); ++i)
		{
			PTOKEN_USER pTUser = users[i];

			if (pTUser != NULL)
			{
				delete[] pTUser;
				pTUser = NULL;
			}
		}

		return true;
	}

	bool LogonSessionsInformationHelper::analyze(CPRecord* parentRecord)
	{
		LogonSessionsInformation logoninfo;
		GetLogonSessionsInformation(logoninfo);
		if (logoninfo.empty()) {
			return false;
		}

		for (auto it = logoninfo.begin(); it != logoninfo.end(); it++)
		{
			LogonSessionInformation& logoninfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"LogonSession") = logoninfo.logon_session_;
			record->at(L"UserName") = logoninfo.user_name_;
			record->at(L"AuthPackage") = logoninfo.auth_package_;
			record->at(L"Session") = logoninfo.session_;
			record->at(L"LogonType") = logoninfo.logon_type_;
			record->at(L"SID") = logoninfo.sid_;
			record->at(L"LogonTime") = logoninfo.logon_time_;
			record->at(L"LogonServer") = logoninfo.logon_server_;
			record->at(L"DNSDomain") = logoninfo.dns_domain_;
			record->at(L"UPN") = logoninfo.upn_;
			if (!logoninfo.logon_server_.empty()){
				for (auto itr = logoninfo.processlist_.begin(); itr != logoninfo.processlist_.end(); itr++){
					record->at(L"ProcessID") = itr->first;
					record->at(L"Path") = itr->second;
				}
			}
		
		}
		return true;

	}
}
