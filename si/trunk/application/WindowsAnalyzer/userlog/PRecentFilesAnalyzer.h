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

		//����ע���
		static void getRegInfos(std::vector<RecentInfo>& infos);
		static void parseSubkey(const HKEY& rootKey, const std::wstring& subKeyName, std::vector<RecentInfo>& infos);
		static bool parseValueItem(const HKEY& key, uint index, RecentInfo& info);
		static std::wstring parseItemPath(const HKEY& key, const std::wstring& valueName);

		//���������Ŀ¼
		static void getRecentFolderInfos(std::vector<RecentInfo>& infos);
		static void getRecentFolders(std::vector<std::wstring>& recentFolders);
		static bool parseLnkItem(const std::wstring& lnkPath, RecentInfo& info);
	};
}