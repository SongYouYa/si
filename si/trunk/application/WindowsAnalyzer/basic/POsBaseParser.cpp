#include "stdafx.h"
#include "POsBaseParser.h"
#include "./material/PRegParserUtils.h"

namespace app
{
	extern std::mutex g_fsMutex;

	bool CPOsBaseParser::getOsBaseInfo(PSIHANDLE volume, OsBaseInfo& osBaseInfo)
	{
		std::lock_guard<std::mutex> lock(g_fsMutex);

		if (!parseSystem(volume, osBaseInfo)) {
			return false;
		}
		if (!parseSoftware(volume, osBaseInfo)) {
			return false;
		}
		return true;
	}

	bool CPOsBaseParser::parseSystem(PSIHANDLE volume, OsBaseInfo& osBaseInfo)
	{
		const std::wstring SystemRegPath = L"windows/system32/config/system";

		PSIHANDLE sysHandle = CFileSystem::GetItemHandleByRawPath(volume, SystemRegPath);
		if (!sysHandle) {
			return false;
		}

		CPFsRegFilePtr regFilePtr = std::make_shared<CPFsRegFile>(sysHandle);
		CPRegFile* regFile = regFilePtr.get();

		std::wstring computerKeyPath = CPRegParserUtils::getSystemCurrentSetName(regFile);
		if (computerKeyPath.empty()) {
			return false;
		}
		computerKeyPath.append(L"\\Control\\ComputerName\\ComputerName");
		SIPKEY osKey;
		if (!CPRegistryParser::openKey(regFile, computerKeyPath, osKey)) {
			return false;
		}

		CPRegistryValue regValue;
		if (!CPRegistryParser::getValue(osKey, L"ComputerName", &regValue)) {
			return false;
		}
		osBaseInfo.computerName = regValue.toString().c_str();

		CPRegistryParser::closeKey(osKey);

		return true;
	}

	bool CPOsBaseParser::parseSoftware(PSIHANDLE volume, OsBaseInfo& osBaseInfo)
	{
		const std::wstring SoftwareRegPath = L"windows/system32/config/software";
		PSIHANDLE softHandle = CFileSystem::GetItemHandleByRawPath(volume, SoftwareRegPath);
		if (!softHandle) {
			return false;
		}

		CPFsRegFilePtr regFilePtr = std::make_shared<CPFsRegFile>(softHandle);
		CPRegFile* regFile = regFilePtr.get();

		std::wstring keyPath = L"Microsoft\\Windows NT\\CurrentVersion";
		SIPKEY softKey;
		if (!CPRegistryParser::openKey(regFile, keyPath, softKey)) {
			return false;
		}

		CPRegistryValue regValue;
		if (!CPRegistryParser::getValue(softKey, L"ProductName", &regValue)) {
			return false;
		}
		osBaseInfo.osProductName = regValue.toString().c_str();
		if (CPRegistryParser::getValue(softKey, L"CurrentType", &regValue)) {
			osBaseInfo.osCurrentType = regValue.toString().c_str();
		}
		if (CPRegistryParser::getValue(softKey, L"SystemRoot", &regValue)) {
			osBaseInfo.systemRoot = regValue.toString().c_str();
#ifdef OS_PWINDOWS
			//替换为当前挂载的盘符
			if (osBaseInfo.systemRoot.size() > 2) {
				std::wstring volumePath = CFileSystem::GetItemPath(volume);
				if (boost::filesystem::exists(volumePath)) { //是否正常路径
					osBaseInfo.systemRoot.replace(0, 2, volumePath);//把盘符替换为当前盘符
				}
			}
#endif
		}

		if (CPRegistryParser::getValue(softKey, L"InstallDate", &regValue)) {
			uint installTimeStamp = regValue.toUint();
			osBaseInfo.installDate = CPTime::UTC2LocalTime((time64_t)installTimeStamp,
				CPTime::GetCurrentTimeZoneBias());
		}
		if (CPRegistryParser::getValue(softKey, L"RegisteredOwner", &regValue)) {
			osBaseInfo.regOwner = regValue.toString().c_str();
		}
		if (CPRegistryParser::getValue(softKey, L"CurrentVersion", &regValue)) {
			osBaseInfo.osProductVersion = regValue.toString().c_str();
		}
		if (CPRegistryParser::getValue(softKey, L"CSDVersion", &regValue)) {
			osBaseInfo.osServiceVersion = regValue.toString().c_str();
		}

		parseIeVersion(regFile, osBaseInfo.ieVersion);

		CPRegistryParser::closeKey(softKey);

		return true;
	}

	bool isMaxVersion(const std::wstring& strVersion, int& maxVersion)
	{
		
		int nVersion = 0;
		try {
			size_t pos = strVersion.find(L".");
			if (pos != std::string::npos) {
				nVersion = boost::lexical_cast<int>(strVersion.substr(0, pos));
			}
		}
		catch (...) {
			return false;
		}

		if (nVersion > maxVersion) {
			maxVersion = nVersion;
			return true;
		}
		return false;
	}

	void CPOsBaseParser::parseIeVersion(CPRegFile* regFile, std::wstring& ieVersion)
	{
		std::vector<std::wstring> ieKeyPaths;
		ieKeyPaths.push_back(L"Microsoft\\Internet Explorer");
		ieKeyPaths.push_back(L"Wow6432Node\\Microsoft\\Internet Explorer");

		CPRegistryValue regValue;
		int maxVersion = 0;
		for (auto& keyPath : ieKeyPaths) {
			SIPKEY ieKey;
			if (!CPRegistryParser::openKey(regFile, keyPath, ieKey)) {
				continue;
			}
			if (CPRegistryParser::getValue(ieKey, L"Version", &regValue)) {
				std::wstring strVersion = regValue.toString().c_str();
				if (isMaxVersion(strVersion, maxVersion)) {
					ieVersion = strVersion;
				}
			}
			if (CPRegistryParser::getValue(ieKey, L"svcVersion", &regValue)) {
				std::wstring strVersion = regValue.toString().c_str();
				if (isMaxVersion(strVersion, maxVersion)) {
					ieVersion = strVersion;
				}
			}
			if (CPRegistryParser::getValue(ieKey, L"W2kVersion", &regValue)) {
				std::wstring strVersion = regValue.toString().c_str();
				if (isMaxVersion(strVersion, maxVersion)) {
					ieVersion = strVersion;
				}
			}
			CPRegistryParser::closeKey(ieKey);
		}
	}

	bool CPOsBaseParser::getOsBaseInfoRecord(PSIHANDLE volume, CPRecord* parentRecord)
	{
		OsBaseInfo osBaseInfo;
		if (!getOsBaseInfo(volume, osBaseInfo)) {
			return false;
		}
		CPRecord* record = parentRecord->addChild();
		osBaseInfo.toRecord(record);
		return true;
	}
}