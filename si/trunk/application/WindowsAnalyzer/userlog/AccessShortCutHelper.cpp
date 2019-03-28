#include "stdafx.h"
#include "AccessShortcutHelper.h"

#pragma comment(lib, "advapi32.lib")

namespace app
{
	AccessShortcutHelper::AccessShortcutHelper(void)
	{
	}
	AccessShortcutHelper::~AccessShortcutHelper(void)
	{
	}
	bool AccessShortcutHelper::GetAccessShortcuts(vecAccessShortcutInformation &vecInformation)
	{
		HKEY keyUserAssist;
		DWORD dwValueNameMaxLen, dwValueDataMaxLen, dwEntryNum;
		DWORD dwValueIndex;
		DWORD dwValueNameSize, dwValueDataSize;
		TCHAR * tszValueName;
		BYTE * pbyteValueData;
		DWORD dwType;
		SYSTEMTIME stRunTime;
		FILETIME ftLocal;
		BOOL fOdd = TRUE;
		AccessShortcut singleRecord;
		if (RegOpenKeyEx(HKEY_CURRENT_USER,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{75048700-EF1F-11D0-9888-006097DEACF9}\\Count",
			NULL, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &keyUserAssist) == ERROR_SUCCESS)
		{
			//获得最大Value和最大DataSize
			RegQueryInfoKey(keyUserAssist,
				NULL, NULL, // Class
				NULL,	// Reserved
				NULL, // Subkey counts
				NULL, // SubKey Name Size
				NULL, // longest class string
				&dwEntryNum, // number of value entries
				&dwValueNameMaxLen,
				&dwValueDataMaxLen,
				NULL, NULL);	// Decriptior , Last Write Time
			dwValueDataMaxLen = dwValueDataMaxLen;
			tszValueName = new TCHAR[dwValueNameMaxLen];
			pbyteValueData = new BYTE[dwValueDataMaxLen];
			dwValueNameSize = dwValueNameMaxLen;
			dwValueDataSize = dwValueDataMaxLen;
			dwValueIndex = 0;
			memset(tszValueName, '\0', dwValueNameMaxLen * 2);
			while (RegEnumValue(keyUserAssist,
				dwValueIndex,
				tszValueName,
				&dwValueNameSize,
				NULL,
				&dwType,
				pbyteValueData,
				&dwValueDataSize) != ERROR_NO_MORE_ITEMS)
			{
				//UEME_RUNPIDL
				if ((wcsncmp(tszValueName, L"HRZR_EHACVQY", wcslen(L"HRZR_EHACVQY")) == 0) &&
					wcslen(tszValueName) != wcslen(L"HRZR_EHACVQY"))
				{
					ROT13Ex(tszValueName, dwValueNameSize);
					FileTimeToLocalFileTime((FILETIME *)(pbyteValueData + 8), &ftLocal);
					FileTimeToSystemTime(&ftLocal, &stRunTime);
					std::wstring wstrLinkPath = tszValueName + wcslen(L"HRZR_EHACVQY:");
					singleRecord.link_path_ = wstrLinkPath;
					TCHAR tszCache[256];
					if (stRunTime.wYear != 1601){
						swprintf(tszCache, 100, L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
							stRunTime.wYear,
							stRunTime.wMonth,
							stRunTime.wDay,
							stRunTime.wHour,
							stRunTime.wMinute,
							stRunTime.wSecond,
							stRunTime.wMilliseconds);
					}
					else	{
						wcscpy_s(tszCache, L"N/A");
					}
					singleRecord.access_time_ = tszCache;
					vecInformation.push_back(singleRecord);
				}
				dwValueNameSize = dwValueNameMaxLen;
				dwValueDataSize = dwValueDataMaxLen;
				dwValueIndex++;
				memset(tszValueName, '\0', dwValueNameMaxLen * 2);
			}
			RegCloseKey(keyUserAssist);
			delete[]tszValueName;
			delete[]pbyteValueData;
		}
		if (RegOpenKeyEx(HKEY_CURRENT_USER,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{5E6AB780-7743-11CF-A12B-00AA004AE837}\\Count",
			NULL,
			KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
			&keyUserAssist) == ERROR_SUCCESS)
		{
			//获得最大Value和最大DataSize
			RegQueryInfoKey(keyUserAssist,
				NULL, NULL, // Class
				NULL,	// Reserved
				NULL, // Subkey counts
				NULL, // SubKey Name Size
				NULL, // longest class string
				&dwEntryNum, // number of value entries
				&dwValueNameMaxLen,
				&dwValueDataMaxLen,
				NULL, NULL);	// Decriptior , Last Write Time
			dwValueDataMaxLen = dwValueDataMaxLen;
			tszValueName = new TCHAR[dwValueNameMaxLen];
			pbyteValueData = new BYTE[dwValueDataMaxLen];
			dwValueNameSize = dwValueNameMaxLen;
			dwValueDataSize = dwValueDataMaxLen;
			dwValueIndex = 0;
			memset(tszValueName, '\0', dwValueNameMaxLen * 2);
			while (RegEnumValue(keyUserAssist,
				dwValueIndex,
				tszValueName,
				&dwValueNameSize,
				NULL,
				&dwType,
				pbyteValueData,
				&dwValueDataSize) != ERROR_NO_MORE_ITEMS)
			{
				//UEME_RUNPIDL
				if ((wcsncmp(tszValueName, L"HRZR_EHACVQY", wcslen(L"HRZR_EHACVQY")) == 0) &&
					wcslen(tszValueName) != wcslen(L"HRZR_EHACVQY"))
				{
					ROT13Ex(tszValueName, dwValueNameSize);
					FileTimeToLocalFileTime((FILETIME *)(pbyteValueData + 8), &ftLocal);
					FileTimeToSystemTime(&ftLocal, &stRunTime);
					std::wstring wstrLinkPath = tszValueName + wcslen(L"HRZR_EHACVQY:");
					singleRecord.link_path_ = wstrLinkPath;
					TCHAR tszCache[256];
					if (stRunTime.wYear != 1601){
						swprintf(tszCache, 100, L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
							stRunTime.wYear,
							stRunTime.wMonth,
							stRunTime.wDay,
							stRunTime.wHour,
							stRunTime.wMinute,
							stRunTime.wSecond,
							stRunTime.wMilliseconds);
					}
					else	{
						wcscpy_s(tszCache, L"N/A");
					}
					singleRecord.access_time_ = tszCache;
					vecInformation.push_back(singleRecord);
				}
				dwValueNameSize = dwValueNameMaxLen;
				dwValueDataSize = dwValueDataMaxLen;
				memset(tszValueName, '\0', dwValueNameMaxLen * 2);
				dwValueIndex++;
			}
			delete[]tszValueName;
			delete[]pbyteValueData;
			RegCloseKey(keyUserAssist);
		}
		//windows 7
		if (RegOpenKeyEx(HKEY_CURRENT_USER,
			L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist\\{F4E57C4B-2036-45F0-A9AB-443BCFE33D9F}\\Count",
			NULL,
			KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS,
			&keyUserAssist) == ERROR_SUCCESS)
		{
			WCHAR dir[MAX_PATH];
			GetSystemDirectory(dir, MAX_PATH);
			std::wstring  systemRootDir = dir;
			systemRootDir =systemRootDir.substr(0, 3);
			std::wstring system32Dir = systemRootDir+L"windows\\system32";
			std::wstring progamDir = systemRootDir+L"Program Files";
			std::wstring windowsDir = systemRootDir+L"windows";
			std::wstring startDir = systemRootDir+L"ProgramData\\Microsoft\\Windows\\Start Menu\\Programs";
			std::wstring defaultUserDir = systemRootDir+L"Users\\Default\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs";
			system32Dir.at(0) = dir[0];
			progamDir.at(0) = dir[0];
			windowsDir.at(0) = dir[0];
			startDir.at(0) = dir[0];
			//获得最大Value和最大DataSize
			RegQueryInfoKey(keyUserAssist,
				NULL, NULL, // Class
				NULL,	// Reserved
				NULL, // Subkey counts
				NULL, // SubKey Name Size
				NULL, // longest class string
				&dwEntryNum, // number of value entries
				&dwValueNameMaxLen,
				&dwValueDataMaxLen,
				NULL, NULL);	// Decriptior , Last Write Time
			dwValueDataMaxLen = dwValueDataMaxLen;
			tszValueName = new TCHAR[dwValueNameMaxLen];
			pbyteValueData = new BYTE[dwValueDataMaxLen];
			dwValueNameSize = dwValueNameMaxLen;
			dwValueDataSize = dwValueDataMaxLen;
			dwValueIndex = 0;
			memset(tszValueName, '\0', dwValueNameMaxLen * 2);
			while (RegEnumValue(keyUserAssist,
				dwValueIndex,
				tszValueName,
				&dwValueNameSize,
				NULL,
				&dwType,
				pbyteValueData,
				&dwValueDataSize) != ERROR_NO_MORE_ITEMS)
			{
				ROT13Ex(tszValueName, dwValueNameSize);
				FileTimeToLocalFileTime((FILETIME *)(pbyteValueData + 60), &ftLocal);
				FileTimeToSystemTime(&ftLocal, &stRunTime);

				std::wstring temp = tszValueName;
				if (temp.size() == 0 || temp == L"UEME_CTLSESSION" || temp == L"UEME_CTLCUACount:ctor")	{
					dwValueNameSize = dwValueNameMaxLen;
					dwValueDataSize = dwValueDataMaxLen;
					memset(tszValueName, '\0', dwValueNameMaxLen * 2);
					dwValueIndex++;
					continue;
				}
				else if (temp.find(L"{D65231B0-B2F1-4857-A4CE-A8E7C6EA7D27}") == 0){
					temp.replace(0, 38, system32Dir);
				}
				else if (temp.find(L"{7C5A40EF-A0FB-4BFC-874A-C0F2E0B9FA8E}") == 0){
					temp.replace(0, 38, progamDir);
				}
				else if (temp.find(L"{F38BF404-1D43-42F2-9305-67DE0B28FC23}") == 0){
					temp.replace(0, 38, windowsDir);
				}
				else if (temp.find(L"{0139D44E-6AFE-49F2-8690-3DAFCAE6FFB8}") == 0){
					temp.replace(0, 38, startDir);
				}
				else if (temp.find(L"{A77F5D77-2E2B-44C3-A6A2-ABA601054A51}") == 0){
					temp.replace(0, 38, defaultUserDir);
				}
				singleRecord.link_path_ = temp;
				TCHAR tszCache[256];
				if (stRunTime.wYear != 1601){
					swprintf(tszCache, 100, L"%04d-%02d-%02d %02d:%02d:%02d.%03d",
						stRunTime.wYear,
						stRunTime.wMonth,
						stRunTime.wDay,
						stRunTime.wHour,
						stRunTime.wMinute,
						stRunTime.wSecond,
						stRunTime.wMilliseconds);
				}
				else{
					wcscpy_s(tszCache, L"N/A");
				}
				singleRecord.access_time_ = tszCache;
				vecInformation.push_back(singleRecord);
				dwValueNameSize = dwValueNameMaxLen;
				dwValueDataSize = dwValueDataMaxLen;
				memset(tszValueName, '\0', dwValueNameMaxLen * 2);
				dwValueIndex++;
			}
			delete[]tszValueName;
			delete[]pbyteValueData;
			RegCloseKey(keyUserAssist);
		}
		return true;
	}

	bool AccessShortcutHelper::analyze(CPRecord* parentRecord)
	{
		vecAccessShortcutInformation vecShortcuts;
		GetAccessShortcuts(vecShortcuts);
		if (vecShortcuts.empty()) {
			return false;
		}

		for (auto it = vecShortcuts.begin(); it != vecShortcuts.end(); it++)
		{
			AccessShortcut& vecShortcuts = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"LinkPath") = vecShortcuts.link_path_;
			record->at(L"AccessTime") = vecShortcuts.access_time_;
		}
		return true;
	}
	void AccessShortcutHelper::ROT13Ex(TCHAR * tszBuffer, DWORD dwLen)
	{
		TCHAR * TChTemp = tszBuffer;
		for (DWORD i = 0; i < dwLen; i++)
		{
			if ((*TChTemp >= L'A' && *TChTemp < L'N') || (*TChTemp >= L'a' && *TChTemp < L'n'))
				(*TChTemp) += 13;
			else if ((*TChTemp >= L'N' && *TChTemp <= L'Z') || (*TChTemp >= L'n' && *TChTemp <= L'z'))
				(*TChTemp) -= 13;
			TChTemp++;
		}

	}
}