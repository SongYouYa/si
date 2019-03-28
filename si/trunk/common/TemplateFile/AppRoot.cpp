#include "stdafx.h"
#include "AppRoot.h"
#include "PathRegex.h"
#include "AppTarget.h"

#include "siutility/FileSearch.h"
#include "FileAnalyzer/PLnkFile.h"

namespace tpfile
{
	const std::wstring LnkExt = L"lnk";

	CAppRoot::CAppRoot()
		: parent_(NULL)
	{

	}

	CAppRoot::~CAppRoot()
	{
		std::vector<CAppTarget*>::iterator it = appTargets_.begin();
		for(; it != appTargets_.end(); ++it)
		{
			if(*it)
			{
				delete *it;
			}
		}
	}

	void CAppRoot::SetRootPath(const std::wstring& rootPath)
	{
//        if(rootPath.empty())
//		{
//			regexPath = L".*"; //空表示匹配所有路径
//		}
        rootPath_ = rootPath;
        rootRegex_ = boost::xpressive::wsregex::compile(rootPath, boost::xpressive::icase);
	}

    std::wstring CAppRoot::RootPath()
    {
        return rootPath_;
    }

	void CAppRoot::SetBaseDir(const std::wstring& baseDir)
	{
        baseDir_ = baseDir;
	}

    std::wstring CAppRoot::BaseDir()
    {
        return baseDir_;
    }

	void CAppRoot::SetSubKeyNames( const std::wstring& subKeyNames )
	{
		subKeyNames_ = subKeyNames;
	}

    void CAppRoot::SearchFiles()
	{
        SearchRoot();
		PickFile();
	}

    void CAppRoot::SearchRoot()
	{
        std::vector<PSIHANDLE> rootHandles;
        siutil::CFileSearch cfs(baseDir_, rootPath_, keyNames_);
		bool bRet = cfs.SetSubKeyNames(subKeyNames_);
        cfs.GetHandles(rootHandles);
        realRootPath_.swap(rootHandles);
	}

	void CAppRoot::PickFile()
	{
        if(appTargets_.empty()) {
            return;
        }
        for (auto it = realRootPath_.begin(); it != realRootPath_.end();) {
			PSIHANDLE rootItem = *it;
			bool bPick = false;
			for (size_t j = 0; j < appTargets_.size(); j++) {
				CAppTarget* pTarget = appTargets_.at(j);
				//在root目录下，逐个查找每个target
				if (PickTargetFile(pTarget,rootItem))
				{
					bPick = true;
				}
			}
			if (!bPick) //没有匹配到文件,则删除root路径
			{
				it = realRootPath_.erase(it);
			}
			else if(it != realRootPath_.end())
			{
				++it;
			}
		}
	}

	bool CAppRoot::PickTargetFile(CAppTarget* pTarget, PSIHANDLE rootItem)
	{
		std::vector<CPathRegex*> vPathRegex;
		pTarget->GetPathRegex(vPathRegex);

		int childCount = CFileSystem::SubItemCount(rootItem);
		bool bPick = false;
		for (int i = 0; i < childCount; ++i) {
			PSIHANDLE subItem = CFileSystem::GetSubItemHandle(rootItem, i);
			for (size_t j = 0; j < vPathRegex.size(); ++j) {
				CPathRegex* pPathRegex = vPathRegex.at(j);
				if (PickPathRegexFile(pPathRegex, subItem)) {
					bPick = true; //至少成功匹配到一个文件
				}
			}
		}
		return bPick;
	}

	bool CAppRoot::PickPathRegexFile(CPathRegex* pPathRegex, PSIHANDLE subRootItem)
	{
		std::wstring itemName = CFileSystem::GetItemName(subRootItem);
		if (!pPathRegex->IsMatch(itemName)) {
			return false;
		}
		else if (pPathRegex->IsDir() && CFileSystem::IsFolder(subRootItem)) {//文件夹
#if 0
			if(CFileSystem::IsItemDeleted(subRootItem)) {
				return false;
			}
#endif
			pPathRegex->AddRealPath(subRootItem);

			//子项的匹配
			std::vector<CPathRegex*> vSubPathRegex;
			pPathRegex->GetSubPathRegex(vSubPathRegex);
			int childCount = CFileSystem::SubItemCount(subRootItem);
            bool bPick = false; //判断子节点是否有成功匹配到文件
			for (int i = 0;  i < childCount; i++) {
				PSIHANDLE  subItem = CFileSystem::GetSubItemHandle(subRootItem, i);
				for (size_t j = 0; j < vSubPathRegex.size(); ++j) {
					CPathRegex* pSubPathRegex = vSubPathRegex.at(j);
					if (PickPathRegexFile(pSubPathRegex, subItem)) {
						bPick = true; //有成功匹配到一个
					}
				}
			}
            return bPick || childCount > 0 && vSubPathRegex.size() == 0; //配置文件中，没有子项时，则提取整个目录
		}
		else if ( !pPathRegex->IsDir() && !CFileSystem::IsFolder(subRootItem))//文件
		{
#if 0
			if(CFileSystem::IsItemDeleted(subRootItem)) //已删除文件
			{
				return false;
			}
#endif
			std::wstring extName = CFileSystem::GetFileExtName(subRootItem);
			//若是lnk文件，则找到真实文件
			if (CPString::isEquali(extName, LnkExt)) {
#ifdef OS_PWINDOWS
				PSIHANDLE realHandle = subRootItem;
				pfile::CPLnkFile lnkFile(subRootItem);
				std::wstring realPath = lnkFile.targetPath();//获取真实路径
				realHandle = CFileSystem::GetItemHandleByPath(realPath);
				//获取失败或是文件夹则不算
				if (!realHandle || CFileSystem::IsFolder(realHandle)) {
					return false;
				}
                //若是lnk文件添加的是lnk的目标handle
                pPathRegex->AddRealPath(realHandle);
#else
				return false;
#endif
			}
			else {
				pPathRegex->AddRealPath(subRootItem);
			}

			return true;
		}

		return false;
	}

	void CAppRoot::AddAppTarget( CAppTarget* pAppTarget )
	{
		appTargets_.push_back(pAppTarget);
	}

	size_t CAppRoot::AppTargetCount()
	{
		return appTargets_.size();
	}

	void CAppRoot::SetParent( CTemplateApp* parent )
	{
		parent_ = parent;
	}

	CTemplateApp* CAppRoot::GetParent()
	{
		return parent_;
	}

	void CAppRoot::SetOsType(const std::wstring& osType)
	{
		osType_ = osType;
	}

	void CAppRoot::SetVersion(const std::wstring& version)
	{
		version_ = version;
	}

	std::wstring CAppRoot::OsType()
	{
		return osType_;
	}

	std::wstring CAppRoot::Version()
	{
		return version_;
	}

	void CAppRoot::SetName(const std::wstring& name)
	{
		name_ = name;
	}

	std::wstring CAppRoot::Name()
	{
		return name_;
	}

	void CAppRoot::SetKeyNames(const std::wstring& keyNames)
	{
		keyNames_ = keyNames;
	}

	void CAppRoot::GetSearchedHandles( std::vector<PSIHANDLE>& handles )
	{
        if(appTargets_.empty())
        {
            handles.insert(handles.end(), realRootPath_.begin(), realRootPath_.end());
            return;
        }

		std::vector<CAppTarget*>::iterator vt = appTargets_.begin();
		for(; vt != appTargets_.end(); ++vt)
		{
			std::vector<PSIHANDLE> subHandles;
			(*vt)->GetSearchedHandles(subHandles);
			handles.insert(handles.end(), subHandles.begin(), subHandles.end());
		}
	}
}
