#pragma once
#include "material/PTable.h"

namespace report
{
	//一些生成报告通用的处理函数
	class CPTable;
	class CPNormalRow;
	class CPMultiRow;
	class CPCell;
	class CPReportHandler
	{
	public:

		static void Record2NormalRow(CPRecord* record, CPNormalRow* pRow,
			bool withMedia);
		static void Record2KvTable(CPRecord* record, CPTableBase* table);
		static bool outputTemplate(TemplateDictionary* dict, const std::wstring& tplName,
			const std::wstring& outPath);
		static void getHeaderData(CPRecordType* recordType, TableHeadDatas& headDatas);
		static void getKvHeaderData(TableHeadDatas& headDatas);
	private:
		static void setCell(CPRecord* record, size_t i, CPNormalRow* pRow, CPCell* cell,
			const CPRecordField* filed, bool withMedia);
		static void setMediaCell(CPRecord* record, CPNormalRow* pRow, size_t index);
		static std::wstring getMediaSrc(CPRecord* record, CPNormalRow* pRow, size_t index,
			std::wstring& ext);
	};
}
