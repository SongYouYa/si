#include "stdafx.h"
#include "ProcessInfoHelper.h"

#include <Tlhelp32.h>
#include <Psapi.h>
#include <tchar.h>

#include "SetPrivilege.h"

#pragma comment(lib, "Version.lib")

namespace app
{
	bool ProcessInfoHelper::GetProcessListInformation(ProcessInformation& process_info)
	{
		HANDLE hProcessSnap;
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap == INVALID_HANDLE_VALUE){
			return false;
		}
		// Set the size of the structure before using it.
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);

		if (!Process32First(hProcessSnap, &pe32)){
			CloseHandle(hProcessSnap);     // Must clean up the snapshot object!
			return false;
		}

		HANDLE hToken = INVALID_HANDLE_VALUE;
		if (CPPrivailegeSeting::GetCurrentThreadPrivilegeToken(&hToken)){
			if (!CPPrivailegeSeting::SetPrivilege(hToken, SE_DEBUG_NAME, TRUE)){
				CloseHandle(hToken);
			}
		}
		do{
			HANDLE hProcess;
			ProcessRecord process;
			process.process_id_ = pe32.th32ProcessID;
			process.process_name_ = pe32.szExeFile;
			process.thread_count_ = pe32.cntThreads;
			WCHAR process_path[MAX_PATH] = L"";
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			DWORD dwSessionID = 0;
			DWORD pdwHandleCount = 0;
			DWORD dwUserObjectCount = 0;
			std::wstring username = L"";
			std::wstring exeLogicPath = L"";
			std::wstring strDescription = L"";
			process.memory_workingset_size_ = 0;
			if (hProcess != NULL){
				ProcessIdToSessionId(pe32.th32ProcessID, &dwSessionID);//会话ID
				GetProcessHandleCount(hProcess, &pdwHandleCount);//获取句柄数
				LPCTSTR pUserName = GetProcessUserName(hProcess); //获取用户名
				if (pUserName) //此处需判断指针是否有效，若无效指针，std::string会崩溃
				{
					username = pUserName;
				}
				dwUserObjectCount = GetGuiResources(hProcess, GR_USEROBJECTS);//用户对象，此值在任务管理器中为动态变化值
				GetProcessImageFileNameW(hProcess, process_path, MAX_PATH);
				exeLogicPath = DosDevicePath2LogicalPath(process_path);//设备路径转为逻辑盘路径
				strDescription = GetFileVersion(exeLogicPath); //获取exe描述信息

				PROCESS_MEMORY_COUNTERS_EX memoryInfo;
				memoryInfo.cb = sizeof(memoryInfo);
				GetProcessMemoryInfo(hProcess, (PPROCESS_MEMORY_COUNTERS)&memoryInfo, sizeof(memoryInfo));
				process.memory_workingset_size_ = memoryInfo.WorkingSetSize;

				CloseHandle(hProcess);
			}
			process.session_id_ = dwSessionID;
			process.handleCount_ = pdwHandleCount;
			process.userName_ = username;
			process.userObject_ = dwUserObjectCount;
			process.description_ = strDescription;
			process.path_ = exeLogicPath;

			process_info.push_back(process);
		} while (Process32Next(hProcessSnap, &pe32));
		CPPrivailegeSeting::SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);
		CloseHandle(hToken);
		CloseHandle(hProcessSnap);
		return true;
	}

	std::wstring ProcessInfoHelper::DosDevicePath2LogicalPath(LPCTSTR lpszDosPath)
	{
		std::wstring strResult;

		// Translate path with device name to drive letters.
		TCHAR szTemp[MAX_PATH];
		szTemp[0] = '\0';

		if (lpszDosPath == NULL || !GetLogicalDriveStrings(_countof(szTemp) - 1, szTemp)){
			return strResult;
		}
		TCHAR szName[MAX_PATH];
		TCHAR szDrive[3] = TEXT(" :");
		BOOL bFound = FALSE;
		TCHAR* p = szTemp;

		do{
			// Copy the drive letter to the template string
			*szDrive = *p;

			// Look up each device name
			if (QueryDosDevice(szDrive, szName, _countof(szName))){
				UINT uNameLen = (UINT)_tcslen(szName);

				if (uNameLen < MAX_PATH)
				{
					bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

					if (bFound){
						// Reconstruct pszFilename using szTemp
						// Replace device path with DOS path
						TCHAR szTempFile[MAX_PATH];
						_stprintf_s(szTempFile, TEXT("%s%s"), szDrive, lpszDosPath + uNameLen);
						strResult = szTempFile;
					}
				}
			}

			// Go to the next NULL character.
			while (*p++);
		} while (!bFound && *p); // end of string

		return strResult;
	}


	LPCTSTR ProcessInfoHelper::GetProcessUserName(HANDLE hProcess)
	{
		if (hProcess == NULL)
			return NULL;

		HANDLE hToken = NULL;
		BOOL bResult = FALSE;
		DWORD dwSize = 0;

		static TCHAR szUserName[256] = { 0 };
		TCHAR szDomain[256] = { 0 };
		DWORD dwDomainSize = 256;
		DWORD dwNameSize = 256;

		SID_NAME_USE    SNU;
		PTOKEN_USER pTokenUser = NULL;
		__try
		{
			if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
			{
				bResult = FALSE;
				__leave;
			}

			if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
			{
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				{
					bResult = FALSE;
					__leave;
				}
			}

			pTokenUser = NULL;
			pTokenUser = (PTOKEN_USER)malloc(dwSize);
			if (pTokenUser == NULL)
			{
				bResult = FALSE;
				__leave;
			}

			if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
			{
				bResult = FALSE;
				__leave;
			}

			if (LookupAccountSid(NULL, pTokenUser->User.Sid, szUserName, &dwNameSize, szDomain, &dwDomainSize, &SNU) != 0)
			{
				return szUserName;
			}
		}
		__finally
		{
			if (pTokenUser != NULL)
				free(pTokenUser);
		}

		return NULL;

	}
	std::wstring ProcessInfoHelper::GetFileVersion(const std::wstring& strFilePath)
	{
		DWORD dwSize;
		DWORD dwRtn;
		std::wstring szVersion;
		//获取版本信息大小
		dwSize = GetFileVersionInfoSize(strFilePath.c_str(), NULL);
		if (dwSize == 0)
		{
			return L"";
		}
		TCHAR *pBuf;
		pBuf = new TCHAR[dwSize + 1];
		if (pBuf == NULL)
			return L"";
		memset(pBuf, 0, dwSize + 1);
		//获取版本信息
		dwRtn = GetFileVersionInfo(strFilePath.c_str(), NULL, dwSize, pBuf);
		if (dwRtn == 0)
		{
			return L"";
		}
		LPVOID lpBuffer = NULL;
		UINT uLen = 0;
		//版本资源中获取信息

		dwRtn = VerQueryValue(pBuf,
			TEXT("\\StringFileInfo\\040904B0\\FileDescription"), //获取英文描述  //040904B0  080404b0      
			&lpBuffer,
			&uLen);
		if (dwRtn == 0)
		{
			dwRtn = VerQueryValue(pBuf,
				TEXT("\\StringFileInfo\\080404b0\\FileDescription"), //0804获取中文描述  //040904B0        
				&lpBuffer,
				&uLen);

		}
		if (dwRtn == 0)
		{
			return L"";
		}
		szVersion = (TCHAR*)lpBuffer;
		delete pBuf;
		return szVersion;
	}

	bool ProcessInfoHelper::analyze(CPRecord* parentRecord)
	{
		ProcessInformation  processinfo;
		GetProcessListInformation(processinfo);

		if (processinfo.empty()) {
			return false;
		}

		for (auto it = processinfo.begin(); it != processinfo.end(); it++)
		{
			ProcessRecord& logoninfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"PID") = logoninfo.process_id_;
			record->at(L"Name") = logoninfo.process_name_;
			record->at(L"UserName") = logoninfo.userName_;
			record->at(L"WorkingSetSize") = logoninfo.memory_workingset_size_;
			record->at(L"SessionID") = logoninfo.session_id_;
			record->at(L"HandleCount") = logoninfo.handleCount_;
			record->at(L"ThreadCount") = logoninfo.thread_count_;
			record->at(L"UserObject") = logoninfo.userObject_;
			record->at(L"Path") = logoninfo.path_;
			record->at(L"Description") = logoninfo.description_;	
		}
		return true;
	}
}
