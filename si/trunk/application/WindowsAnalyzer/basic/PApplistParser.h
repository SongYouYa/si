#pragma once


namespace app
{
	struct AppInstallInfo
	{
		std::wstring displayname;
		std::wstring publisher;
		std::wstring version;
		std::wstring url;

		void toRecord(CPRecord* record)
		{
			record->at(L"displayname") = displayname;
			record->at(L"publisher") = publisher;
			record->at(L"version") = version;
			record->at(L"url") = url;
		}

		bool operator <(const AppInstallInfo& r) const
		{
			if (displayname < r.displayname) {
				return true;
			}
			else if (displayname > r.displayname) {
				return false;
			}
			else {
				if (version < r.version) {
					return true;
				}
				else if (version > r.version) {
					return false;
				}
			}
			return false;
		}
	};
	typedef std::vector<AppInstallInfo> AppInstallInfos;

	class CPApplistParser
	{
	public:
		//isMicro: 系统应用
		static bool getApplist(PSIHANDLE volume, CPRecord* parentRecord, bool isMicro);

	private:
		static bool parseAppInfos(PSIHANDLE volume, AppInstallInfos& appInfos);
	};
}