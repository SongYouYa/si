#include "stdafx.h"
#include "PPageSet.h"
#include "PPage.h"
#include "PPageJumper.h"
#include "PTable.h"
#include "handler/PReportHandler.h"

const size_t PAGE_ROW_COUNT = 500;

namespace report
{
	CPPageSet::CPPageSet(const std::vector<CPRecord*>& records, const std::wstring& outDir,
		int level, const std::wstring& rootDir, bool withMedia)
		: _records(records),
		_outDir(outDir), _level(level),
		_rootDir(rootDir),
		_withMedia(withMedia)
	{
		
	}

	bool CPPageSet::output()
	{
		size_t count = _records.size();
		if (count == 0) {
			return false;
		}

		std::string upperDir;
		for (int i = 0; i < _level; ++i) {
			upperDir += "../";
		}

		std::map<uint, std::wstring> index_filenames;
		CPPageJumper::getIndexFilenames(count, PAGE_ROW_COUNT, index_filenames);

		size_t columnCount = 0;
		TableHeadDatas headDatas;
		CPRecord* firstRecord = _records.at(0);
		if (firstRecord->isFolder()) {
			columnCount = 1;  //如果是目录，则列只有一行
			TableHeadData headData;
			headData.data_ = GET_TEXT(TEXT_ID_NAME);
			headDatas.push_back(headData);
		}
		else {
			columnCount = firstRecord->getType()->publicFieldCount();
			CPReportHandler::getHeaderData(firstRecord->getType(), headDatas);
		}

		size_t recordIndex = 0;
		uint fileIndex = 1;
		for (; fileIndex <= index_filenames.size(); ++fileIndex) {
			auto it = index_filenames.find(fileIndex);
			if (it == index_filenames.end()) {
				continue;
			}
			std::wstring fileName = it->second;

			std::wstring filePath = _outDir + L"/" + fileName;
			CPPage* page = new CPPage(this, filePath);
			page->setUpperDir(upperDir);
			CPTable* table = page->addTable(columnCount, nullptr, recordIndex);
			table->SetHead(headDatas);
			addRecords2Table(recordIndex, PAGE_ROW_COUNT, table);
			if (index_filenames.size() > 1) {
				TemplateDictionary* pageJumpDict = page->addPageJumpDict();
				CPPageJumper::setPageJump(pageJumpDict, index_filenames, fileIndex);
			}
			page->output();
			page->release();

			recordIndex += PAGE_ROW_COUNT;
		}
		return true;
	}

	void CPPageSet::addRecords2Table(size_t beginIndex, size_t count, CPTable* pTable)
	{
        size_t endIndex = pmin((beginIndex + count), _records.size());
		for (size_t i = beginIndex; i < endIndex; ++i)
		{
			CPRecord* record = _records.at(i);
			CPNormalRow* pNormalRow = pTable->AddNormalRow();
			CPReportHandler::Record2NormalRow(record, pNormalRow, _withMedia);
		}
	}

	std::wstring CPPageSet::getRootDir()
	{
		return _rootDir;
	}
}
