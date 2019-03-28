#include "stdafx.h"
#include "PContent2.h"
#include "handler/PReportHandler.h"
#include "PReportPath.h"
#include "material/PTable2.h"

extern std::wstring g_productName;

namespace report
{
	extern std::string IdPre = "_ID_"; //和树导航中的名字一致

	CPContent2::CPContent2(CPRecord* rootRecord)
		: _rootRecord(rootRecord)
	{
		_tablesDict = new TemplateDictionary("");
	}

	CPContent2::~CPContent2()
	{
		for (auto it = _tables.begin(); it != _tables.end(); ++it)
		{
			delete *it;
		}
	}

	bool CPContent2::output(const std::wstring& filePath)
	{
		_tablesDict->SetValue("HTML_TITLE", W2U8(g_productName));
		addTables(_rootRecord);
		bool ret = CPReportHandler::outputTemplate(_tablesDict, L"content2.tpl", filePath);
		return ret;
	}

	void CPContent2::addTables(CPRecord* record)
	{
		if (record->flags() & RFlagNoReport) {
			return;
		}
		if (record->childrenCount() == 0) {
			//落款特殊处理一下
			if (record->getType()->name() == L"table.reportsign") {
				TemplateDictionary* signDict = _tablesDict->AddSectionDictionary("SIGNS");
				signDict->SetValue("ARCHOR_NAME", IdPre + I2S(record->id()));
			}
			return;
		}

		TemplateDictionary* pCataDict = nullptr;
		if (record->level() > 0) {
			std::wstring folderName = record->getNodeName();
			pCataDict = _tablesDict->AddSectionDictionary("CATOGARY");
			TemplateDictionary* pTitleDict = pCataDict->AddSectionDictionary("TITLE");
			pTitleDict->SetValue("LEVEL", I2S(/*record->level()*/3)); //标题大小统一为
			pTitleDict->SetValue("TEXT", W2U8(folderName));
			pTitleDict->SetValue("ARCHOR_NAME", IdPre + I2S(record->id()));
		}

		CPRecord* firstChild = record->child(0);
		if (firstChild->isFolder()) {
			size_t count = record->childrenCount();
			for (size_t i = 0; i < count; ++i) {
				CPRecord* child = record->child(i);
				addTables(child);
			}
		}
		else if (pCataDict) {
			TemplateDictionary* pInDict = pCataDict->AddSectionDictionary("INCL_TABLE");
			TemplateDictionary* pTableDict = pInDict->AddIncludeDictionary("INCL_TPL");
			std::wstring confDir = CPReportPath::GetConfRootDir();
			std::string tableTplPath = W2LS(confDir + L"/table.tpl");
			pTableDict->SetFilename(tableTplPath);
			CPTable2* pTable = addRecords2Table(record, pTableDict);
			if (pTable) {
				_tables.push_back(pTable);
			}
		}
	}

	CPTable2* CPContent2::addRecords2Table(CPRecord* folderRecord, TemplateDictionary* pTableDict)
	{
		size_t childrenCount = folderRecord->childrenCount();
		if (childrenCount == 0) {
			return nullptr;
		}
		CPRecord* firstChild = folderRecord->child(0);
		CPTable2* pTable = nullptr; 

		if (childrenCount > 1) {
			size_t columnCount = firstChild->getType()->publicFieldCount();
			pTable = new CPTable2(pTableDict, columnCount);
			TableHeadDatas headDatas;
			CPReportHandler::getHeaderData(firstChild->getType(), headDatas);
			pTable->SetHead(headDatas);
			for (size_t i = 0; i < childrenCount; ++i) {
				CPRecord* child = folderRecord->child(i);
				CPNormalRow* pNormalRow = pTable->AddNormalRow();
				CPReportHandler::Record2NormalRow(child, pNormalRow, false);
			}
		}
		else {
			//当只有一行时，使用键值列
			pTable = new CPTable2(pTableDict, 2);
			TableHeadDatas headDatas;
			CPReportHandler::getKvHeaderData(headDatas);
			pTable->SetHead(headDatas);
			CPReportHandler::Record2KvTable(firstChild, pTable);
		}
		//由于在qt界面中，预览时，css设置的一些样式不起作用，需要对table另外设置属性
		pTable->addProperty("width=\"100%\" border=\"1\" cellspacing=\"0\" cellpadding=\"4\"");
		pTable->addHeadRowProperty("bgcolor=\"#F3F3F3\"");
		pTable->addHeadNumCellProperty("width=\"45\"");
		return pTable;
	}
}
