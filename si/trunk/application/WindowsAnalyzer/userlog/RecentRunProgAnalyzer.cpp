#include "stdafx.h"
#include "RecentRunProgAnalyzer.h"

#include "siwinregistry/PRegistry.h"

#include <boost/xpressive/xpressive.hpp>

namespace app
{
	RecentRunProgAnalyzer::RecentRunProgAnalyzer(void)
	{

	}


	RecentRunProgAnalyzer::~RecentRunProgAnalyzer(void)
	{
	}

	bool RecentRunProgAnalyzer::analyze(CPRecord *parentRecord)
	{
		vecRecentRunInformaion vecRecentRuns;
		ParseRunMruRecords(vecRecentRuns);
		bool ret = ParseUserAssist(vecRecentRuns);
		if (!ret){
			return false;
		}

		for (auto it = vecRecentRuns.begin(); it != vecRecentRuns.end(); ++it) {
			CPRecord* serviceRecord = parentRecord->addChild();
			serviceRecord->at(L"ProgramPath") = it->program_path_;
			serviceRecord->at(L"RunTime") = it->run_time_;
			serviceRecord->at(L"AccessCount") = it->access_count_;
		}
		return true;
	}

	void RecentRunProgAnalyzer::ParseRunMruRecords(vecRecentRunInformaion &vecInformation)
	{
		DWORD dwValueNameMaxLen, dwValueDataMaxLen, dwSubKeys, dwSubkeyNameSize, dwEntryNum;
		LONG lRtn;
		FILETIME ftKey;

		HKEY keyRunMru;

		if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RUNMRU",
			0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &keyRunMru) != ERROR_SUCCESS)
		{
			return;
		}
		RegQueryInfoKey(keyRunMru,
			NULL, NULL, // Class
			NULL,	// Reserved
			&dwSubKeys, // Subkey counts
			&dwSubkeyNameSize, // SubKey Name Size
			NULL, // longest class string
			&dwEntryNum, // number of value entries
			&dwValueNameMaxLen,
			&dwValueDataMaxLen,
			NULL, &ftKey);	// Decriptior , Last Write Time
		dwValueDataMaxLen = dwValueDataMaxLen;


		FILETIME ftLocal;
		FileTimeToLocalFileTime(&ftKey, &ftLocal);
		//		singleRecord.run_time_ = ftLocal;

		TCHAR * pTChValueName;
		TCHAR * pTChValueData;
		TCHAR * pTChMRUList;

		pTChValueData = new TCHAR[dwValueDataMaxLen];
		pTChValueName = new TCHAR[dwValueNameMaxLen];
		pTChMRUList = new TCHAR[dwValueDataMaxLen / 2 + 1];

		DWORD dwDataSize = dwValueDataMaxLen;
		DWORD dwType;

		if ((lRtn = RegQueryValueEx(keyRunMru, L"MRUList",
			NULL, &dwType, (LPBYTE)pTChMRUList, &dwDataSize)) != ERROR_SUCCESS)
		{
			RegCloseKey(keyRunMru);
			delete[]pTChValueName;
			delete[]pTChValueData;
			delete[]pTChMRUList;
			return;
		}

		//		singleRecord.program_path_ = L"RUNMRU";
		//		singleRecord.access_count_ = 0;
		//		vecInformation.push_back(singleRecord);

		TCHAR * pTChTemp = pTChMRUList;
		TCHAR * pTChTempData;
		DWORD dwValueDataSize;
		DWORD dwNameSize;

		RecentRunProgram singleRecord;

		BOOL fOdd = FALSE;
		while (*pTChTemp != L'\0')
		{
			memset(pTChValueName, '\0', dwValueNameMaxLen);
			dwValueDataSize = dwValueDataMaxLen;
			dwNameSize = dwValueNameMaxLen;

			*pTChValueName = *pTChTemp;
			if ((lRtn = RegQueryValueEx(keyRunMru, pTChValueName,
				NULL, &dwType, (LPBYTE)pTChValueData, &dwValueDataSize)) != ERROR_SUCCESS)
			{
				delete[]pTChValueName;
				delete[]pTChValueData;
				delete[]pTChMRUList;
				RegCloseKey(keyRunMru);
				return;
			}
			pTChTempData = pTChValueData;
			for (DWORD i = 0; i < (dwValueDataSize + 1) / 2 - 1; i++)
			{
				if (*pTChTempData == L'\0')
					*pTChTempData = L' ';
				pTChTempData++;
			}
			pTChTemp++;

			singleRecord.program_path_ = pTChValueData;
			size_t pos = singleRecord.program_path_.rfind(L"\\1"); //ȥ��ע��������ġ�\1���ַ���
			if (pos != std::string::npos)
			{
				singleRecord.program_path_ = singleRecord.program_path_.substr(0, pos);
			}
			singleRecord.access_count_ = 0;
			singleRecord.run_time_ = (time64_t)-1; //���ʱ��
			vecInformation.push_back(singleRecord);
		}

		delete[]pTChValueName;
		delete[]pTChValueData;
		delete[]pTChMRUList;

		RegCloseKey(keyRunMru);
		return;
	}

	bool RecentRunProgAnalyzer::ParseUserAssist(vecRecentRunInformaion& recentProgInfos)
	{
		bool bOsVerVistaLater = psystem::CPOsVersion::isVistaOrLater(); //�Ƿ���windows Vista֮��ϵͳ�汾

		if (bOsVerVistaLater)
		{
			//��ʼ������GUID�ַ�����
			bool ret = _kownFolder.init();
			if (!ret)
			{
				return false;
			}
		}

		//�˼��¼�¼�����еĳ���
		const std::wstring USER_ASSIST_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\UserAssist";
		const std::wstring COUNT_KEYNAME = L"Count"; //�Ӽ���

		HKEY userAssistkey;
		bool ret = winreg::CPRegistry::OpenKey(HKEY_CURRENT_USER, USER_ASSIST_KEY, userAssistkey);
		if (!ret)
		{
			return false;
		}

		//��ȡUserAssist����һ��������ΪGUID�ַ���
		std::vector<std::wstring> guidKeyNames;
		ret = winreg::CPRegistry::GetSubKeyNames(userAssistkey, guidKeyNames);
		if (!ret)
		{
			return false;
		}

		for (std::vector<std::wstring>::iterator it = guidKeyNames.begin(); it != guidKeyNames.end(); ++it)
		{
			std::wstring& guidKeyName = *it;
			std::wstring countKeyName = guidKeyName + L"\\" + COUNT_KEYNAME;
			//��Count�Ӽ�
			HKEY countKey;
			ret = winreg::CPRegistry::OpenKey(userAssistkey, countKeyName, countKey);
			if (!ret)
			{
				continue;
			}
			//��ȡCount��������Value����
			std::vector<std::wstring> valueNames;
			ret = winreg::CPRegistry::GetValueNames(countKey, valueNames);
			if (!ret)
			{
				continue;
			}

			for (std::vector<std::wstring>::iterator it_vn = valueNames.begin(); it_vn != valueNames.end(); ++it_vn)
			{
				std::wstring& valueName = *it_vn;
				std::vector<byte> valueData;
				ret = winreg::CPRegistry::ReadValueData(countKey, valueName, valueData);
				if (ret)
				{
					RecentRunProgram oneRecord;
					if (bOsVerVistaLater)
					{
						ret = ParseValueDataVistaLater(valueName, valueData, oneRecord);
					}
					else
					{
						ret = ParseValueDataXp(valueName, valueData, oneRecord);
					}

					if (ret)
					{
						recentProgInfos.push_back(oneRecord);
					}
				}
			}
			winreg::CPRegistry::CloseKey(countKey);
		}
		winreg::CPRegistry::CloseKey(userAssistkey);
		return true;
	}

	bool RecentRunProgAnalyzer::ParseValueDataVistaLater(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord)
	{
		if (valueData.size() != 72) //�����ļ�¼�������ݴ�СΪ72
		{
			return false;
		}

		std::wstring progPath = valueName; //Value������Ϊ����·��
		DecodeRot13((TCHAR*)progPath.c_str(), progPath.size()); //Rot13����

		//�滻GUID�ַ���Ϊʵ��·��
		const std::wstring strRegex = L"\\{[0-9a-f]{8}(-[0-9a-f]{4}){3}-[0-9a-f]{12}\\}";
		const boost::xpressive::wsregex wreg = boost::xpressive::wsregex::compile(strRegex, boost::xpressive::icase);
		boost::xpressive::wsmatch what;
		if (boost::xpressive::regex_search(progPath, what, wreg))
		{
			std::wstring strGuid = what[0].str();
			std::wstring realPath = _kownFolder.getRealPath(strGuid);
			if (!realPath.empty())
			{
				progPath = boost::xpressive::regex_replace(progPath, wreg, realPath);
			}
			else
			{
				std::wstring temp;
			}
		}
		else
		{
			std::wstring temp;
		}
		oneRecord.program_path_ = progPath;

		DWORD numExecute = 0;
		memcpy(&numExecute, &valueData[4], 4); //Value���ݵĵ�4~7�ֽ�Ϊ����ִ�д���
		oneRecord.access_count_ = numExecute;

		FILETIME lastRunTime;
		memcpy(&lastRunTime, &valueData[60], 8); //���ݵĵ�60~67�ֽ�Ϊ���ִ��ʱ��
		FileTimeToLocalFileTime(&lastRunTime, &lastRunTime);
		oneRecord.run_time_ = lastRunTime;

		return true;
	}
	void RecentRunProgAnalyzer::DecodeRot13(TCHAR * tszBuffer, DWORD dwLen)
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
		return;
	}
	bool RecentRunProgAnalyzer::ParseValueDataXp(const std::wstring& valueName, const std::vector<byte>& valueData, RecentRunProgram& oneRecord)
	{
		if (valueData.size() != 16) //�����ļ�¼�������ݴ�СΪ16
		{
			return false;
		}

		std::wstring progPath = valueName; //Value������Ϊ����·��
		DecodeRot13((TCHAR*)progPath.c_str(), progPath.size()); //Rot13����

		//ȥ���ַ�����ʼ�ġ�UEME_RUNPATH:��
		const std::wstring wstrRep = L"UEME_RUNPATH:";
		if (progPath.find(wstrRep) == 0) //���ַ�����ʼ�ҵ�
		{
			progPath = progPath.substr(wstrRep.size());
		}
		oneRecord.program_path_ = progPath; //xpϵͳ��,���ý���GUID�滻

		DWORD numExecute = 0;
		memcpy(&numExecute, &valueData[4], 4); //Value���ݵĵ�4~7�ֽ�Ϊ����ִ�д���
		oneRecord.access_count_ = (numExecute >= 5) ? numExecute - 5 : numExecute; //��Ϊ�����Ǵ�5��ʼ������Ҫ��ȥ5

		FILETIME lastRunTime;
		memcpy(&lastRunTime, &valueData[8], 8); //���ݵĵ�8~15�ֽ�Ϊ���ִ��ʱ��
		FileTimeToLocalFileTime(&lastRunTime, &lastRunTime);
		oneRecord.run_time_ = lastRunTime;

		return true;
	}

}
