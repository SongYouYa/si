#pragma once
#include "./PRow.h"

namespace report
{
	struct TableHeadData
	{
		std::string data_;
		std::string class_;
		std::string property_;
	};
	typedef std::vector<TableHeadData> TableHeadDatas;

	class CPTableBase
	{
	public:
		CPTableBase(TemplateDictionary* pTableDict, uint columnCount, bool bNumCol);
		virtual ~CPTableBase();

		virtual void addTitle(const std::string& imgSrc, const std::string& text);
		void SetHead(const TableHeadDatas& headDatas);

		//添加不同类型的row
		CPNormalRow* AddNormalRow();
		CPMultiRow* AddMultiRow();

		size_t rowCount();

	protected:
		static ctemplate::TemplateDictionary* CreateRowDict(ctemplate::TemplateDictionary* pTableDict);
		static void addHeadCellProperty(ctemplate::TemplateDictionary* pCellDict,
			const std::string& text);

		ctemplate::TemplateDictionary* _pHeadDict;
		TemplateDictionary* _pHeadNumCellDict;
		ctemplate::TemplateDictionary* _pTableDict;

		std::vector<ctemplate::TemplateDictionary*> headDicts_;

		uint columnCount_;
		size_t rowCount_;
		std::vector<CPRow*> pRows_;
		bool bNumCol_; //是否显示序号列

		static int s_archorNumber; //用于产生锚接的序号
	};
}