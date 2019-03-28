#pragma once

#include "PCell.h"

namespace report
{
	class CPTableBase;
	class CPRow
	{
	protected:
        CPRow(uint64 index, uint columnCount);
	public:
        virtual ~CPRow();
		uint64 Index();
		virtual CPCell* GetCell(uint column) = 0; //从0开始
		uint ColumnCount();

		void SetTable(CPTableBase* pTable);
		CPTableBase* Table();

	protected:
		std::vector<CPCell*> pCells_;
		CPCell* pNumCell_; //序号cell
		uint64 index_; //从0开始
		uint columnCount_;

		static CPCell* CreateCell(ctemplate::TemplateDictionary* pRowDict, CPRow* pRow);
		static ctemplate::TemplateDictionary* CreateRowDict(ctemplate::TemplateDictionary* pTableDict);
	private:
		CPTableBase* pTable_;
	};

	class CPNormalRow : public CPRow
	{
		friend class CPTableBase;
		//标准表格，确定列数, 不包括序号列
		CPNormalRow(ctemplate::TemplateDictionary* pRowDict, uint64 index, uint columnCount, bool bNumCol);
	public:
		~CPNormalRow();
		
		CPCell* GetCell(uint column);

	private:
		ctemplate::TemplateDictionary* pRowDict_;
	};

	//非标准行列，项目名和内容在同一行，并且可能有多个
	class CPMultiRow : public CPRow
	{
		struct RowInfo
		{
			ctemplate::TemplateDictionary* pRowDict_;
			CPCell* pLastCell_; //此行最后一个cell
			uint col_; //此行当前可用列
			RowInfo() : pRowDict_(NULL), pLastCell_(NULL), col_(0) {}
		};
		friend class CPTableBase;
		//pTableDict:为row所在的table的dict
		//columnCount为序号列之外的其他列数,一般为偶数
		//bool bNumCol : 是否创建序号列
		CPMultiRow(ctemplate::TemplateDictionary* pTableDict, uint64 index, uint columnCount, bool bNumCol);
	public:
//		typedef std::map<std::string, int> Map_Name_Span; //项目名和列跨度
		
		~CPMultiRow();

		//只能在最开始添加RowSpanCell
		CPCell* AddRowSpanCell();
		//colSpan 为content Cell所跨的列
		virtual CPCell* AddCell(const std::string& itemName, int colSpan = 1);
		//根据增加的cell，设置IndexCell和其他一些跨cell的跨度
		void Finish();
		virtual CPCell* GetCell(uint column); //获取第几个内容cell，从0开始

	private:
		RowInfo& GetRowInfo(uint colSpan);

	private:
		ctemplate::TemplateDictionary* pTableDict_;

		ctemplate::TemplateDictionary* pCurRowDict_; //当前行dict
		int rowCount_; //创建的row数量

		std::vector<CPCell*> pRowSpanCells_; //跨列cell

		std::vector<RowInfo> vecRowInfos_; //保存行、行当前列

		std::vector<CPCell*> pContentCells_; //内容cell
		
	};
}
