#include "stdafx.h"
#include "AppTarget.h"
#include "PathRegex.h"

namespace tpfile
{
	CAppTarget::CAppTarget()
		: m_parent(NULL)
	{
	}

	CAppTarget::~CAppTarget()
	{
		std::vector<CPathRegex*>::iterator it = m_subPathRegex.begin();
		for(; it != m_subPathRegex.end(); ++it)
		{
			if(*it)
			{
				delete *it;
			}
		}
	}

	void CAppTarget::AddPathRegex( CPathRegex* pRegExpPath )
	{
		m_subPathRegex.push_back(pRegExpPath);
	}

	void CAppTarget::SetName( std::wstring name )
	{
		m_Name = name;
	}

	std::wstring CAppTarget::GetName()
	{
		return m_Name;
	}

	void CAppTarget::GetPathRegex( std::vector<CPathRegex*>& subRegExpPath )
	{
		subRegExpPath = m_subPathRegex;
	}

	void CAppTarget::SetParent( CAppRoot* pTemplate )
	{
		m_parent = pTemplate;
	}

	CAppRoot* CAppTarget::GetParent()
	{
		return m_parent;
	}

	size_t CAppTarget::PathRegexCount()
	{
		return m_subPathRegex.size();
	}

	void CAppTarget::GetSearchedHandles(std::vector<PSIHANDLE>& handles)
	{
		std::vector<CPathRegex*>::iterator vt = m_subPathRegex.begin();
		for (; vt != m_subPathRegex.end(); ++vt) {
			(*vt)->GetSearchedHandles(handles);
		}
	}
}
