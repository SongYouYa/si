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
				ProcessIdToSessionId(pe32.th32ProcessID, &dwSessionID);//�ỰID
				GetProcessHandleCount(hProcess, &pdwHandleCount);//��ȡ�����
				LPCTSTR pUserName = GetProcessUserName(hProcess); //��ȡ�û���
				if (pUserName) //�˴����ж�ָ���Ƿ���Ч������Чָ�룬std::string�����
				{
					username = pUserName;
				}
				dwUserObjectCount = GetGuiResources(hProcess, GR_USEROBJECTS);//�û����󣬴�ֵ�������������Ϊ��̬�仯ֵ
				GetProcessImageFileNameW(hProcess, process_path, MAX_PATH);
				exeLogicPath = DosDevicePath2LogicalPath(process_path);//�豸·��תΪ�߼���·��
				strDescription = GetFileVersion(exeLogicPath); //��ȡexe������Ϣ

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
		//��ȡ�汾��Ϣ��С
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
		//��ȡ�汾��Ϣ
		dwRtn = GetFileVersionInfo(strFilePath.c_str(), NULL, dwSize, pBuf);
		if (dwRtn == 0)
		{
			return L"";
		}
		LPVOID lpBuffer = NULL;
		UINT uLen = 0;
		//�汾��Դ�л�ȡ��Ϣ

		dwRtn = VerQueryValue(pBuf,
			TEXT("\\StringFileInfo\\040904B0\\FileDescription"), //��ȡӢ������  //040904B0  080404b0      
			&lpBuffer,
			&uLen);
		if (dwRtn == 0)
		{
			dwRtn = VerQueryValue(pBuf,
				TEXT("\\StringFileInfo\\080404b0\\FileDescription"), //0804��ȡ��������  //040904B0        
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
