#include "stdafx.h"
#include "PApplistParser.h"


namespace app
{
	extern std::mutex g_fsMutex;

	bool CPApplistParser::getApplist(PSIHANDLE volume, CPRecord* parentRecord, bool isMicro)
	{
		AppInstallInfos appInfos;
		if (!parseAppInfos(volume, appInfos)) {
			return false;
		}

		for (auto& appInfo : appInfos) {
			auto pos = appInfo.publisher.find(L"Microsoft");
			if (isMicro && pos != std::string::npos) {
				CPRecord* record = parentRecord->addChild();
				appInfo.toRecord(record);
			}
			else if (!isMicro && pos == std::string::npos) {
				CPRecord* record = parentRecord->addChild();
				appInfo.toRecord(record);
			}
		}
		return true;
	}

	bool CPApplistParser::parseAppInfos(PSIHANDLE volume, AppInstallInfos& appInfos)
	{
		std::lock_guard<std::mutex> lock(g_fsMutex);

		const std::wstring softwareRegPath = L"windows/system32/config/software";
		PSIHANDLE softHandle = CFileSystem::GetItemHandleByRawPath(volume, softwareRegPath);
		if (!softHandle) {
			return false;
		}

		std::vector<PSIHANDLE> softHandles;
		softHandles.push_back(softHandle);

		const std::wstring ntuserPath7 = L"Users/*/NTUSER.DAT";//win7..
		const std::wstring ntuserPathXP = L"Documents and Settings/*/NTUSER.DAT";//windows xp
		CFileSystem::GetItemHandlesByRawPath(volume, ntuserPath7, softHandles);
		CFileSystem::GetItemHandlesByRawPath(volume, ntuserPathXP, softHandles);

		std::vector<std::wstring> uninstallKeyPaths = {
			L"Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
			L"Microsoft\\Windows\\CurrentVersion\\Uninstall",
			L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
		};

		std::set<AppInstallInfo> setInfos;

		for (auto handle : softHandles) {
			for (auto& uninsKeyPath : uninstallKeyPaths) {
				CPFsRegFilePtr regFilePtr = std::make_shared<CPFsRegFile>(handle);
				CPRegFile* regFile = regFilePtr.get();

				SIPKEY uninsKey;
				if (!CPRegistryParser::openKey(regFile, uninsKeyPath, uninsKey)) {
					continue;
				}
				std::vector<std::wstring> subKeyNames;
				CPRegistryParser::getSubKeyNames(uninsKey, subKeyNames);
				for (auto& subKeyName : subKeyNames) {
					SIPKEY appKey;
					if (!CPRegistryParser::openKey(uninsKey,
						subKeyName, appKey)) {
						continue;
					}
					CPRegistryValue regValue;
					if (!CPRegistryParser::getValue(appKey, L"DisplayName", &regValue)) {
						continue;
					}
					AppInstallInfo appInfo;
					appInfo.displayname = regValue.toString();
					if (CPRegistryParser::getValue(appKey, L"DisplayVersion", &regValue)) {
						appInfo.version = regValue.toString();
					}
					if (CPRegistryParser::getValue(appKey, L"Publisher", &regValue)) {
						appInfo.publisher = regValue.toString();
					}
					if (CPRegistryParser::getValue(appKey, L"URLInfoAbout", &regValue)) {
						appInfo.url = regValue.toString();
					}
					CPRegistryParser::closeKey(appKey);

					appInfos.push_back(appInfo);
					//»•µÙ÷ÿ∏¥œÓ
					/*if (setInfos.find(appInfo) == setInfos.end()) {
						appInfos.push_back(appInfo);
						setInfos.insert(appInfo);
					}*/
				}
				CPRegistryParser::closeKey(uninsKey);
			}
		}
		return true;
	}
}