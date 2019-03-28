#include "stdafx.h"
#include "PRecentFilesAnalyzer.h"
#include "boost/lexical_cast.hpp"
#include "boost/filesystem.hpp"

#include <ShlObj.h>
#include <atlbase.h>

#include "sisystem/globals.h"
#include "sisystem/win/POsVersion.h"
#include "sisystem/win/PLnkParser.h"

using namespace psystem;
namespace bfs = boost::filesystem;
using namespace winreg;

namespace app
{
	bool CPRecentFilesAnalyzer::analyze(CPRecord* parentRecord)
	{
		std::vector<RecentInfo> infos;
		getInfos(infos);
		if (infos.empty()) {
			return false;
		}

		for (auto it = infos.begin(); it != infos.end(); ++it) {
			RecentInfo& info = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"FileName") = info.fileName;
			record->at(L"FilePath") = info.filePath;
			record->at(L"ext") = info.ext;
			record->at(L"OpenTime") = info.openTime;
		}
		return true;
	}

	void CPRecentFilesAnalyzer::getInfos(std::vector<RecentInfo>& infos)
	{
		std::map<std::wstring, size_t> path_indexs;

		std::vector<RecentInfo> recentFolderInfos;
		getRecentFolderInfos(recentFolderInfos);
		mergeInfos(recentFolderInfos, path_indexs, infos);

		std::vector<RecentInfo> regInfos;
		getRegInfos(regInfos);
		mergeInfos(regInfos, path_indexs, infos);
	}

	void CPRecentFilesAnalyzer::mergeInfos(std::vector<RecentInfo>& sourceInfos,
		std::map<std::wstring, size_t>& indexs,
		std::vector<RecentInfo>& targetInfos)
	{
		std::map<std::wstring, size_t> path_indexs;
		for (auto it = sourceInfos.begin(); it != sourceInfos.end(); ++it) {
			auto it_path = path_indexs.find(it->filePath);
			if (it_path == path_indexs.end()) {
				path_indexs[it->filePath] = targetInfos.size();
				targetInfos.push_back(*it);
			}
			else {
				//如果之前已存在
				RecentInfo& preInfo = targetInfos.at(it_path->second);
				if (!preInfo.openTime.IsAvail()) {
					//有效时间
					preInfo.openTime = it->openTime;
				}
				else {
					if (it->openTime.IsAvail() && preInfo.openTime != it->openTime) {
						path_indexs[it->filePath] = targetInfos.size();
						targetInfos.push_back(*it);
					}
				}
			}
		}
	}

	void CPRecentFilesAnalyzer::getRegInfos(std::vector<RecentInfo>& infos)
	{
		const std::wstring OpenSaveMruPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU";
		HKEY openSaveRootKey;
		bool ret = CPRegistry::OpenKey(HKEY_CURRENT_USER, OpenSaveMruPath, openSaveRootKey, true);
		if (!ret) {
			return;
		}
		std::vector<std::wstring> subKeyNames;
		ret = CPRegistry::GetSubKeyNames(openSaveRootKey, subKeyNames);
		if (!ret) {
			CPRegistry::CloseKey(openSaveRootKey);
			return;
		}
		for (auto it = subKeyNames.begin(); it != subKeyNames.end(); ++it) {
			std::wstring subKeyName = *it;
			parseSubkey(openSaveRootKey, subKeyName, infos);
		}
		CPRegistry::CloseKey(openSaveRootKey);
	}

	void CPRecentFilesAnalyzer::parseSubkey(const HKEY& rootKey, const std::wstring& subKeyName, std::vector<RecentInfo>& infos)
	{
		HKEY subKey;
		bool sret = CPRegistry::OpenKey(rootKey, subKeyName, subKey);
		if (!sret) {
			return;
		}
		std::vector<uint> indexData;
		sret = CPRegistry::ReadValueData(subKey, L"MRUListEx", indexData);
		if (!sret) {
			return;
		}
		bool firstItem = true;
		for (auto it_i = indexData.begin(); it_i != indexData.end(); ++it_i) {
			uint index = *it_i;
			RecentInfo info;
			if (parseValueItem(subKey, index, info)) {
				if (firstItem) {
					//key的修改时间，为第一条记录的时间
					FILETIME keyModifyTime;
					if (CPRegistry::getKeyModifyTime(subKey, &keyModifyTime)) {
						info.openTime = CPTime::UTC2LocalTime(keyModifyTime, CPTime::GetCurrentTimeZoneBias());
					}
				}
				infos.push_back(info);
			}
			firstItem = false;
		}
		CPRegistry::CloseKey(subKey);
	}

	bool CPRecentFilesAnalyzer::parseValueItem(const HKEY& key, uint index, RecentInfo& info)
	{
		if (index == -1) {
			return false;
		}
		std::wstring strIndex = boost::lexical_cast<std::wstring>(index);
		std::wstring itemPath = parseItemPath(key, strIndex);
		if (itemPath.empty()) {
			return false;
		}

		info.filePath = itemPath;
		info.fileName = CPString::getFileName(itemPath);
		info.ext = CPString::getFileExt(info.fileName);
		return true;
	}

	std::wstring CPRecentFilesAnalyzer::parseItemPath(const HKEY& key, const std::wstring& valueName)
	{
		//通过API函数，来解析读到的value二进制数据，得到路径
		std::vector<byte> data;
		bool ret = CPRegistry::ReadValueData(key, valueName, data);
		if (!ret) {
			return L"";
		}
		CComPtr<IMalloc> spMalloc;
		HRESULT hr = SHGetMalloc(&spMalloc);
		LPITEMIDLIST pidl = (LPITEMIDLIST)spMalloc->Alloc(data.size() * 2);
		if (!pidl) {
			return L"";
		}
		memcpy(pidl, data.data(), data.size());

		std::wstring itemPath;
		wchar_t szPath[MAX_PATH] = { 0 };
		BOOL bret = SHGetPathFromIDListW(pidl, szPath);
		if (bret) {
			itemPath = szPath;
		}
		spMalloc->Free(pidl);
		return itemPath;
	}


	void CPRecentFilesAnalyzer::getRecentFolderInfos(std::vector<RecentInfo>& infos)
	{
		std::vector<std::wstring> recentFolders;
		getRecentFolders(recentFolders);

		for (auto it_f = recentFolders.begin(); it_f != recentFolders.end(); ++it_f) {
			std::wstring& recentFolder = *it_f;
			try
			{
				if (!bfs::is_directory(recentFolder)) {
					continue;
				}

				bfs::directory_iterator end_iter;
				for (bfs::directory_iterator iter(recentFolder); iter != end_iter; ++iter) {
					auto itemPath = iter->path();
					RecentInfo info;
					if (parseLnkItem(itemPath.wstring(), info)) {
						infos.push_back(info);
					}
				}
			}
			catch (...) {
				continue;
			}
			
		}
	}

	void CPRecentFilesAnalyzer::getRecentFolders(std::vector<std::wstring>& recentFolders)
	{
		wchar_t drive;

		WCHAR winDir[MAX_PATH];
		uint ret = GetWindowsDirectory(winDir, MAX_PATH);
		if (ret > 0) {
			drive = winDir[0];
		}
		else {
			return;
		}
		std::wstring postfix;
		std::wstring userRootDir;
		userRootDir += drive;
		userRootDir += L":\\";
		bool vistaLater = CPOsVersion::isVistaOrLater();
		if (vistaLater) {
			userRootDir += L"Users";
			postfix = L"\\AppData\\Roaming\\Microsoft\\Windows\\Recent";
		}
		else {
			userRootDir += L"Documents and Settings";
			postfix = L"\\Recent";
		}



		bfs::path userRootPath = userRootDir;
		bfs::directory_iterator end_iter;
		try {
			bfs::directory_iterator iter(userRootPath);
			for (; iter != end_iter;) {
				auto subPath = iter->path();
				auto recentPath = subPath /= postfix;
				try {
					if (bfs::exists(recentPath) && bfs::is_directory(recentPath)) {
						recentFolders.push_back(recentPath.wstring());
					}
				}
				catch (...) {
					continue;
				}

				try {
					++iter;
				}
				catch (...) {
					break;
				}
			}
		}
		catch (...) {
			return;
		}
		
	}

	bool CPRecentFilesAnalyzer::parseLnkItem(const std::wstring& lnkPath, RecentInfo& info)
	{
		time64_t modifyTime = 0;
		bool ret = CPLnkParser::parseLnk(lnkPath, info.filePath, &modifyTime);
		if (!ret) {
			return false;
		}
		if (info.filePath.empty()) {
			return false;
		}
		info.openTime = CPTime::UTC2LocalTime(modifyTime, CPTime::GetCurrentTimeZoneBias());
		info.fileName = CPString::getFileName(info.filePath);
		info.ext = CPString::getFileExt(info.fileName);
		return ret;
	}
}