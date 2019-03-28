#include "stdafx.h"
#include "PReportHandler.h"
#include "PReportPath.h"
#include "material/PCssDefine.h"
#include "material/PRow.h"
#include "material/PPageSet.h"
#include "material/PPage.h"

#include "base/PRecordHandler.h"
#include "filesystemshell/FileSystemShell.h"
#include "siutility/PFileSiUtils.h"
using namespace filesystem;


const uint FOALDER_TABLE_COLUMN_COUNT = 4;
const std::string IMG_NAME_LOGO = "logo.ico";

const std::wstring ImgExts = L"png,jpg,jpeg,bmp,gif,ico";

namespace report
{
	std::string getDecoratorClass(const CPRecordField* filed, bool isHeader)
	{
		if (filed->type() == CPVariant::TDateTime) {
			if (isHeader) {
				return COLUMN_TIME;
			}
		}
		else {
			CPRecordField::DECORATOR deco = filed->decorator();
			if (deco == CPRecordField::DECORATOR_SIZE) {
				if (isHeader) {
					return COLUMN_SIZE;
				}
			}
			else if (deco == CPRecordField::DECORATOR_NOREPORT) {
				return COLUMN_HIDE;
			}
		}
		return "";
	}

	void CPReportHandler::setCell(CPRecord* record, size_t i, CPNormalRow* pRow,
		CPCell* cell, const CPRecordField* filed, bool withMedia)
	{
		std::wstring value = CPRecordHandler::variantToString(record->at(i),
			filed->decorator(), false);
		CPRecordField::DISPLAY_TYPE displayType = filed->displayType();
		if (displayType == CPRecordField::DISPLAY_TYPE_STRING) {
			//boost::replace_all(value, L"\n", L"<br/>");
			cell->SetData(W2U8(value));
		}
		else if (displayType == CPRecordField::DISPLAY_TYPE_MEDIA) {
			if (withMedia) {
				setMediaCell(record, pRow, i);
			}
		}
		else {
			assert(false);
		}
		std::string cellClass = getDecoratorClass(filed, false);
		if (!cellClass.empty()) {
			cell->SetClass(cellClass);
		}
	}

	void CPReportHandler::Record2NormalRow(CPRecord* record, CPNormalRow* pRow,
		bool withMedia)
	{
		if (record->isFolder()) {
			pRow->GetCell(0)->SetData(W2U8(record->getNodeName()));
		}
		else {
			auto recordType = record->getType();
			size_t fieldCount = recordType->publicFieldCount();
			for (size_t i = 0; i < fieldCount; ++i) {
				const CPRecordField* filed = recordType->getPublicField(i);
				setCell(record, i, pRow, pRow->GetCell(i), filed, withMedia);
			}
		}
	}

	void CPReportHandler::setMediaCell(CPRecord* record, CPNormalRow* pRow, size_t index)
	{
		std::wstring ext;
		std::wstring mediaSrc = getMediaSrc(record, pRow, index, ext);
		if (!mediaSrc.empty()) {
			if (ImgExts.find(ext) != std::string::npos) {
				pRow->GetCell(index)->SetMediaImg(W2U8(mediaSrc));
			}
			else {
				pRow->GetCell(index)->SetMediaCommon(W2U8(mediaSrc), GET_TEXT(TEXT_ID_PREVIEW));
			}
		}
	}

	std::wstring CPReportHandler::getMediaSrc(CPRecord* record, CPNormalRow* pRow, size_t index,
		std::wstring& ext)
	{
		std::wstring mediaSrc;
		CPTableBase* tableBase = pRow->Table();
		CPTable* table = dynamic_cast<CPTable*>(tableBase);
		if (table) {
			CPPage* page = table->page();
			CPPageSet* pageSet = page->pageSet();

			CPVariant& var = record->at(index);
			PSIHANDLE handle = var.Get<PSIHANDLE>();
			ext = CFileSystem::GetFileExtName(handle);

			//export file
			std::wstring fileName = CFileSystem::GetItemName(handle);
			std::wstring mediaDir = pageSet->getRootDir() + L"/media/";
			std::wstring targetPath = CPPath::getNewPath(mediaDir + fileName);
			siutil::CPSiFileUtils::exportFile(handle, targetPath);
			//获取实际的拷贝文件名
			std::wstring newfileName = CPPath::getFileName(targetPath);
			std::string upperDir = page->upperDir();
			mediaSrc = U82W(upperDir) + L"/media/" + newfileName;
		}
		return mediaSrc;
	}

	bool CPReportHandler::outputTemplate(TemplateDictionary* dict, const std::wstring& tplName,
		const std::wstring& outPath)
	{
		try{
			boost::filesystem::path hpath(outPath);
			boost::filesystem::path parentPath = hpath.parent_path();
			if (!boost::filesystem::exists(parentPath) && !parentPath.empty())
			{
				boost::filesystem::create_directories(parentPath);
			}
		}
		catch (...)
		{
			return false;
		}

		std::string output;
		std::wstring confDir = CPReportPath::GetConfRootDir();
        ctemplate::ExpandTemplate(W2LS(confDir + L"/" + tplName), ctemplate::STRIP_WHITESPACE, dict, &output);
        std::ofstream outHtml(W2LS(outPath));
		outHtml.write(output.data(), output.size());
		outHtml.close();

		return true;
	}

	void CPReportHandler::getHeaderData(CPRecordType* recordType, TableHeadDatas& headDatas)
	{
		size_t fieldCount = recordType->publicFieldCount();
		for (size_t i = 0; i < fieldCount; ++i) {
			const CPRecordField* field = recordType->getPublicField(i);
			std::string fieldName = W2U8(field->display());
			TableHeadData data;
			data.data_ = fieldName;
			data.class_ = getDecoratorClass(field, true);
			headDatas.push_back(data);
		}
	}

	void CPReportHandler::getKvHeaderData(TableHeadDatas& headDatas)
	{
		TableHeadData dataKey;
		dataKey.data_ = GET_TEXT(TEXT_ID_KEY);
		dataKey.class_ = COL_KEY;
		dataKey.property_ = "width=\"20%\""; //键列占20%，和模板文件里保持一致
		headDatas.push_back(dataKey);
		TableHeadData dataValue;
		dataValue.data_ = GET_TEXT(TEXT_ID_VALUE);
		headDatas.push_back(dataValue);
	}

	void CPReportHandler::Record2KvTable(CPRecord* record, CPTableBase* pTable)
	{
		CPRecordType* recordType = record->getType();
		size_t fieldCount = recordType->publicFieldCount();
		for (size_t i = 0; i < fieldCount; ++i) {
			const CPRecordField* field = recordType->getPublicField(i);
			if (field->decorator() == CPRecordField::DECORATOR_NOREPORT) {
				continue;
			}
			CPNormalRow* pNormalRow = pTable->AddNormalRow();
			//键
			std::string fieldName = W2U8(field->display());
			pNormalRow->GetCell(0)->SetData(fieldName);
			//值
			CPCell* valueCell = pNormalRow->GetCell(1);
			setCell(record, i, pNormalRow, valueCell, field, false);
		}
	}
}
