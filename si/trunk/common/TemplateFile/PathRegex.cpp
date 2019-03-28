#include "stdafx.h"
#include "PathRegex.h"
#include "AppTarget.h"


namespace tpfile
{
	CPathRegex::CPathRegex()
        :m_RootTarget(NULL), m_Parent(NULL), m_bDir(false)
	{

	}

	CPathRegex::~CPathRegex()
	{
		std::vector<CPathRegex*>::iterator it = m_subRegExpPath.begin();
		for(; it != m_subRegExpPath.end(); ++it)
		{
			delete *it;
		}
	}

	void CPathRegex::AddSubPathRegex( CPathRegex* subPath )
	{
		for (size_t i = 0 ; i < m_subRegExpPath.size(); i++)
		{
			//先判断有没有相同的
			CPathRegex* curPath = m_subRegExpPath[i];
			if (curPath->IsDir() == subPath->IsDir() &&
				curPath->GetName() == subPath->GetName()) {
				return;
			}
		}
		m_subRegExpPath.push_back(subPath);
	}

	void CPathRegex::GetSubPathRegex( std::vector<CPathRegex*>& vSubPath )
	{
		vSubPath = m_subRegExpPath;
	}

	bool CPathRegex::IsDir()
	{
		return m_bDir;
	}

	void CPathRegex::SetDir(bool flag)
	{
		m_bDir = flag;
	}


	void CPathRegex::SetRootTarget( CAppTarget* target )
	{
		m_RootTarget = target;
	}

	CAppTarget* CPathRegex::GetRootTarget()
	{
		return m_RootTarget;
	}

	void CPathRegex::SetParent( CPathRegex* parent )
	{
		m_Parent = parent;
	}

	CPathRegex* CPathRegex::Parent()
	{
		return m_Parent;
	}

	bool CPathRegex::IsMatch(const std::wstring& name)
	{
		return boost::xpressive::regex_match(name,m_Reg);
	}

	size_t CPathRegex::SubPathRegexCount()
	{
		return m_subRegExpPath.size();
	}

	void CPathRegex::SetName(const std::wstring& name)
	{
		name_ = name;
        m_Reg = boost::xpressive::wsregex::compile(name, boost::xpressive::icase);
	}

	std::wstring CPathRegex::GetName()
	{
		return name_;
	}

	bool CPathRegex::AddRealPath(PSIHANDLE itemHandle)
	{
		for(size_t i = 0; i < itemHandles_.size(); ++i)
		{
			if(itemHandles_.at(i) == itemHandle)
			{
				return false; //有相同的handle
			}
		}
		itemHandles_.push_back(itemHandle);
		return true;
	}

	void CPathRegex::GetSearchedHandles( std::vector<PSIHANDLE>& handles )
	{
        for(auto it = itemHandles_.begin(); it != itemHandles_.end(); ++it) {
            PSIHANDLE pItem = *it;
			handles.push_back(pItem);
			//配置文件中，没有子项时，则提取整个目录
            if(CFileSystem::IsFolder(pItem) && m_subRegExpPath.empty()) {
				CFileSystem::GetAllSubItems(pItem, handles, true);
            }
        }
		std::vector<CPathRegex*>::iterator vtr = m_subRegExpPath.begin();
		for (; vtr != m_subRegExpPath.end(); ++vtr) {
			(*vtr)->GetSearchedHandles(handles);
		}
	}
}
