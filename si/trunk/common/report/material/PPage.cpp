#include "stdafx.h"
#include "material/PPage.h"
#include "material/PTable.h"
#include "PReportPath.h"
#include "handler/PReportHandler.h"


namespace report
{
	CPPage::CPPage(CPPageSet* pageSet, const std::wstring& filePath)
		: _filePath(filePath),
		_pageSet(pageSet)
	{
		_pageDict = new TemplateDictionary("");
	}

	CPPage::~CPPage()
	{
		for (auto it = _tables.begin(); it != _tables.end(); ++it)
		{
			delete *it;
		}
	}

	CPPageSet* CPPage::pageSet()
	{
		return _pageSet;
	}

	void CPPage::setUpperDir(const std::string& upperDir)
	{
		_pageDict->SetValue("UPPER_DIR", upperDir);
		_upperDir = upperDir;
	}

	std::string CPPage::upperDir()
	{
		return _upperDir;
	}

	CPTable* CPPage::addTable(uint columnCount, CPTable* pParentTable, size_t rowBeginIndex)
	{
		TemplateDictionary* pInDict = _pageDict->AddSectionDictionary("INCL_TABLE");
		TemplateDictionary* pTableDict = pInDict->AddIncludeDictionary("INCL_TPL");
		std::wstring confDir = CPReportPath::GetConfRootDir();
        std::string tableTplPath = W2LS(confDir + L"/table.tpl");
		pTableDict->SetFilename(tableTplPath);

		CPTable* pTable = new CPTable(pTableDict, columnCount, this, pParentTable, rowBeginIndex);

		_tables.push_back(pTable);
		return pTable;
	}

	TemplateDictionary* CPPage::addPageJumpDict()
	{
		return _pageDict->AddSectionDictionary("PAGE_JUMP");
	}

	std::wstring CPPage::filePath()
	{
		return _filePath;
	}

	bool CPPage::output()
	{
		bool ret = CPReportHandler::outputTemplate(_pageDict, L"page.tpl", _filePath);
		return ret;
	}

	void CPPage::release()
	{
		for (auto it = _tables.begin(); it != _tables.end(); ++it)
		{
			delete *it;
		}
		_tables.clear();
	}
}
