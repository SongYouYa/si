#include "stdafx.h"
#include "PTable2.h"

namespace report
{
	CPTable2::CPTable2(TemplateDictionary* pTableDict, uint columnCount, bool bNumCol)
		: CPTableBase(pTableDict, columnCount, bNumCol)
	{

	}

	void CPTable2::addProperty(const std::string& text)
	{
		TemplateDictionary* pProDict = _pTableDict->AddSectionDictionary("TB_PROPERTY");
		pProDict->SetValue("PVALUE", text);
	}

	void CPTable2::addHeadRowProperty(const std::string& text)
	{
		if (!_pHeadDict) {
			return;
		}
		TemplateDictionary* pProDict = _pHeadDict->AddSectionDictionary("HR_PROPERTY");
		pProDict->SetValue("PVALUE", text);
	}

	void CPTable2::addHeadNumCellProperty(const std::string& text)
	{
		if (!_pHeadNumCellDict) {
			return;
		}
		addHeadCellProperty(_pHeadNumCellDict, text);
	}
}