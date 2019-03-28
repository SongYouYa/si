#pragma once

namespace app
{
	class CPRecentFilesAnalyzer
	{
		struct RecentInfo
		{
			std::wstring fileName;
			std::wstring ext;
			std::wstring filePath;
			CPTime openTime;
			bool operator==(const RecentInfo& r) {
				if (filePath == r.filePath && openTime == r.openTime) {
					return true;
				}
				return false;
			}
		};
	public:
		static bool analyze(CPRecord* rootRecord);

	private:
		static void getInfos(std::vector<RecentInfo>& infos);

		static void mergeInfos(std::vector<RecentInfo>& sourceInfos,
			std::map<std::wstring, size_t>& indexs,
			std::vector<RecentInfo>& targetInfos);

		//解析注册表
		static void getRegInfos(std::vector<RecentInfo>& infos);
		static void parseSubkey(const HKEY& rootKey, const std::wstring& subKeyName, std::vector<RecentInfo>& infos);
		static bool parseValueItem(const HKEY& key, uint index, RecentInfo& info);
		static std::wstring parseItemPath(const HKEY& key, const std::wstring& valueName);

		//解析最近打开目录
		static void getRecentFolderInfos(std::vector<RecentInfo>& infos);
		static void getRecentFolders(std::vector<std::wstring>& recentFolders);
		static bool parseLnkItem(const std::wstring& lnkPath, RecentInfo& info);
	};
}