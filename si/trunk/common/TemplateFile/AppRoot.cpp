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
//			regexPath = L".*"; //�ձ�ʾƥ������·��
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
				//��rootĿ¼�£��������ÿ��target
				if (PickTargetFile(pTarget,rootItem))
				{
					bPick = true;
				}
			}
			if (!bPick) //û��ƥ�䵽�ļ�,��ɾ��root·��
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
					bPick = true; //���ٳɹ�ƥ�䵽һ���ļ�
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
		else if (pPathRegex->IsDir() && CFileSystem::IsFolder(subRootItem)) {//�ļ���
#if 0
			if(CFileSystem::IsItemDeleted(subRootItem)) {
				return false;
			}
#endif
			pPathRegex->AddRealPath(subRootItem);

			//�����ƥ��
			std::vector<CPathRegex*> vSubPathRegex;
			pPathRegex->GetSubPathRegex(vSubPathRegex);
			int childCount = CFileSystem::SubItemCount(subRootItem);
            bool bPick = false; //�ж��ӽڵ��Ƿ��гɹ�ƥ�䵽�ļ�
			for (int i = 0;  i < childCount; i++) {
				PSIHANDLE  subItem = CFileSystem::GetSubItemHandle(subRootItem, i);
				for (size_t j = 0; j < vSubPathRegex.size(); ++j) {
					CPathRegex* pSubPathRegex = vSubPathRegex.at(j);
					if (PickPathRegexFile(pSubPathRegex, subItem)) {
						bPick = true; //�гɹ�ƥ�䵽һ��
					}
				}
			}
            return bPick || childCount > 0 && vSubPathRegex.size() == 0; //�����ļ��У�û������ʱ������ȡ����Ŀ¼
		}
		else if ( !pPathRegex->IsDir() && !CFileSystem::IsFolder(subRootItem))//�ļ�
		{
#if 0
			if(CFileSystem::IsItemDeleted(subRootItem)) //��ɾ���ļ�
			{
				return false;
			}
#endif
			std::wstring extName = CFileSystem::GetFileExtName(subRootItem);
			//����lnk�ļ������ҵ���ʵ�ļ�
			if (CPString::isEquali(extName, LnkExt)) {
#ifdef OS_PWINDOWS
				PSIHANDLE realHandle = subRootItem;
				pfile::CPLnkFile lnkFile(subRootItem);
				std::wstring realPath = lnkFile.targetPath();//��ȡ��ʵ·��
				realHandle = CFileSystem::GetItemHandleByPath(realPath);
				//��ȡʧ�ܻ����ļ�������
				if (!realHandle || CFileSystem::IsFolder(realHandle)) {
					return false;
				}
                //����lnk�ļ���ӵ���lnk��Ŀ��handle
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
