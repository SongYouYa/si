#include "stdafx.h"
#include "PRow.h"
#include "PTable.h"
#include "PCssDefine.h"

using namespace ctemplate;

//const std::string NUM_CELL_CLASS = "numCell";

namespace report
{
    CPRow::CPRow(uint64 index, uint columnCount)
        : index_(index), columnCount_(columnCount), pNumCell_(NULL),
		pTable_(nullptr)
	{
		assert(columnCount > 0);
	}

	CPRow::~CPRow()
	{
		for (std::vector<CPCell*>::iterator it = pCells_.begin(); it != pCells_.end(); ++it)
			delete *it;
		if (pNumCell_)
			delete pNumCell_;
	}

	uint64 CPRow::Index()
	{
		return index_;
	}

	CPCell* CPRow::CreateCell(ctemplate::TemplateDictionary* pRowDict, CPRow* pRow)
	{
		TemplateDictionary* pCellDict = pRowDict->AddSectionDictionary("CELL");
		CPCell* pCell = new CPCell(pCellDict, pRow);
		return pCell;
	}

	ctemplate::TemplateDictionary* CPRow::CreateRowDict(ctemplate::TemplateDictionary* pTableDict)
	{
		return pTableDict->AddSectionDictionary("ROW");
	}

	uint CPRow::ColumnCount()
	{
		return columnCount_;
	}

	void CPRow::SetTable(CPTableBase* pTable)
	{
		pTable_ = pTable;
	}

	CPTableBase* CPRow::Table()
	{
		return pTable_;
	}

	CPNormalRow::CPNormalRow(ctemplate::TemplateDictionary* pRowDict, uint64 index, uint columnCount, bool bNumCol)
        : CPRow(index, columnCount), pRowDict_(pRowDict)
	{
		if (bNumCol)
		{
			pNumCell_ = CreateCell(pRowDict_, this);
			pNumCell_->SetData(index_ + 1);
		}
			
		for (uint i = 0; i < columnCount_; ++i)
		{
			CPCell* pCell = CreateCell(pRowDict_, this);
			pCells_.push_back(pCell);
		}
	}

	CPNormalRow::~CPNormalRow()
	{
		
	}

	CPCell* CPNormalRow::GetCell(uint column)
	{
		assert(column < columnCount_);
		if (column >= columnCount_)
		{
			return NULL;
		}

		return pCells_.at(column);
	}


	CPMultiRow::CPMultiRow(ctemplate::TemplateDictionary* pTableDict, uint64 index, uint columnCount, bool bNumCol)
        : pTableDict_(pTableDict), CPRow(index, columnCount), rowCount_(0), pCurRowDict_(NULL)
	{
		//多行列的列数应为偶数
		assert(columnCount % 2 == 0);

		pCurRowDict_ = CreateRowDict(pTableDict_);
		++rowCount_;
		if (bNumCol)
		{
			//创建序号列
			pNumCell_ = CreateCell(pCurRowDict_, this);
			pNumCell_->SetData(index_ + 1);
			pNumCell_->SetClass(CELL_TYPE_NUM);
			pRowSpanCells_.push_back(pNumCell_);
		}

		RowInfo rowInfo;
		rowInfo.pRowDict_ = pCurRowDict_;
		vecRowInfos_.push_back(rowInfo);
	}

	CPMultiRow::~CPMultiRow()
	{
	}


	CPCell* CPMultiRow::AddCell(const std::string& itemName, int colSpan)
	{
		assert(colSpan > 0);
		uint temp_ColSpan = colSpan;
		//因为内容cell的项目cell占一列,所以这里加1比较
		if (temp_ColSpan + 1 > columnCount_) 
		{
			temp_ColSpan = columnCount_ - 1;
		}

		RowInfo& rowInfo = GetRowInfo(temp_ColSpan);
		rowInfo.pLastCell_ = CreateCell(rowInfo.pRowDict_, this);
		pCells_.push_back(rowInfo.pLastCell_);
		rowInfo.pLastCell_->SetData(itemName); //设置项目cell的内容
		rowInfo.col_ += 1;
		//项目Cell，设置最小列宽
		rowInfo.pLastCell_->SetClass(FIRST_MULTI_CELL);
		
		//内容cell
		rowInfo.pLastCell_ = CreateCell(rowInfo.pRowDict_, this);
		pCells_.push_back(rowInfo.pLastCell_);
		rowInfo.pLastCell_->SetColSpan(temp_ColSpan);
		rowInfo.col_ += temp_ColSpan;
		pContentCells_.push_back(rowInfo.pLastCell_);

		return rowInfo.pLastCell_;
	}

	void CPMultiRow::Finish()
	{
		for (std::vector<CPCell*>::iterator it = pRowSpanCells_.begin(); it != pRowSpanCells_.end(); ++it)
		{
			CPCell* pSpanCell = *it;
			pSpanCell->SetRowSpan(rowCount_);
		}

		//最后列空余的需要设置col
		for (auto it = vecRowInfos_.begin(); it != vecRowInfos_.end(); ++it)
		{
			RowInfo& rowInfo = *it;
			if (rowInfo.col_ < columnCount_ && rowInfo.pLastCell_)
			{
				rowInfo.pLastCell_->SetColSpan(rowInfo.pLastCell_->GetColSpan() + columnCount_ - rowInfo.col_);
			}
		}
		
	}

	CPCell* CPMultiRow::AddRowSpanCell()
	{
		--columnCount_;
		assert(columnCount_ > 0);
		
		CPCell* pRowSpanCell = CreateCell(pCurRowDict_, this);
		pRowSpanCells_.push_back(pRowSpanCell);
		pCells_.push_back(pRowSpanCell);
		
		return pRowSpanCell;
	}

	CPMultiRow::RowInfo& CPMultiRow::GetRowInfo(uint colSpan)
	{
		for (auto it = vecRowInfos_.begin(); it != vecRowInfos_.end();)
		{
			RowInfo& rowInfo = *it;
			if (colSpan + 1 <= (columnCount_ - rowInfo.col_)) //-1 是因为内容cell前的项目cell占用1列
			{
				return rowInfo;
			}

			if (rowInfo.col_ >= columnCount_) //此行已满
			{
				it = vecRowInfos_.erase(it);
			}
			else
			{
				++it;
			}
		}
		
		RowInfo rowInfo;
		rowInfo.pRowDict_ = CreateRowDict(pTableDict_);
		vecRowInfos_.push_back(rowInfo);
		++rowCount_;
		return vecRowInfos_.back(); //注意要返回vector里的不要返回局部变量
	}

	CPCell* CPMultiRow::GetCell(uint column)
	{
		assert(column < columnCount_);
		assert(column < pContentCells_.size());
		return pContentCells_.at(column);
	}

}
