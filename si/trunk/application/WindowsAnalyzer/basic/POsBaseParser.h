#pragma once


namespace app
{
	struct OsBaseInfo
	{
		std::wstring computerName;
		std::wstring osCurrentType;
		std::wstring osProductName;
		std::wstring regOwner;
		std::wstring systemRoot;
		CPTime installDate;
		std::wstring osProductVersion;
		std::wstring osServiceVersion;
		std::wstring ieVersion;

		void toRecord(CPRecord* record)
		{
			record->at(L"ComputerName") = computerName;
			record->at(L"SystemVersion") = osCurrentType + L" " + osProductName;
			record->at(L"RegisterOwner") = regOwner;
			record->at(L"SystemRoot") = systemRoot;
			record->at(L"InstallDate") = installDate;
			record->at(L"IEVersion") = ieVersion;
			record->at(L"ProductVersion") = osProductVersion;
			record->at(L"ServerVersion") = osServiceVersion;
		}
	};
	typedef std::vector<OsBaseInfo> OsBaseInfos;

	class CPOsBaseParser
	{
	public:
		static bool getOsBaseInfo(PSIHANDLE volume, OsBaseInfo& osBaseInfo);
		static bool getOsBaseInfoRecord(PSIHANDLE volume, CPRecord* parentRecord);

	private:
		static bool parseSystem(PSIHANDLE volume, OsBaseInfo& osBaseInfo);
		static bool parseSoftware(PSIHANDLE volume, OsBaseInfo& osBaseInfo);
		static void parseIeVersion(CPRegFile* regFile, std::wstring& ieVersion);
	};
}