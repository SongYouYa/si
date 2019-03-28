#include "stdafx.h"
#include "PTableBase.h"

#include "PCssDefine.h"


const std::string HEAD_CELL_DATA = "CELL_DATA";

namespace report
{
	int CPTableBase::s_archorNumber = 0;

	CPTableBase::CPTableBase(TemplateDictionary* pTableDict, uint columnCount,
		bool bNumCol)
		: _pHeadDict(nullptr), _pHeadNumCellDict(nullptr),
		_pTableDict(pTableDict), columnCount_(columnCount),
		rowCount_(0), bNumCol_(bNumCol)
	{

	}

	CPTableBase::~CPTableBase()
	{
		for (std::vector<CPRow*>::iterator it = pRows_.begin(); it != pRows_.end(); ++it)
		{
			delete *it;
		}
	}

	void CPTableBase::addTitle(const std::string& imgSrc, const std::string& text)
	{

	}

	void CPTableBase::SetHead(const TableHeadDatas& headDatas)
	{
		assert(headDatas.size() <= columnCount_);
		_pHeadDict = _pTableDict->AddSectionDictionary("HEAD");

		if (bNumCol_)
		{
			_pHeadNumCellDict = _pHeadDict->AddSectionDictionary("CELL");
			_pHeadNumCellDict->SetValue(HEAD_CELL_DATA, GET_TEXT(TEXT_ID_NUM));
			_pHeadNumCellDict->SetValue("CLASS_NAME", CELL_TYPE_NUM);
		}

		for (size_t i = 0; i < columnCount_; ++i)
		{
			TemplateDictionary* pCellDict = _pHeadDict->AddSectionDictionary("CELL");
			if (i < headDatas.size())
			{
				const TableHeadData& headData = headDatas.at(i);
				pCellDict->SetValue(HEAD_CELL_DATA, headData.data_);
				pCellDict->SetValue("CLASS_NAME", headData.class_);
				if (!headData.property_.empty()) {
					addHeadCellProperty(pCellDict, headData.property_);
				}
			}
			headDicts_.push_back(pCellDict);
		}
	}

	CPNormalRow* CPTableBase::AddNormalRow()
	{
		TemplateDictionary* pRowDict = CreateRowDict(_pTableDict);
		CPNormalRow* pNorRow = new CPNormalRow(pRowDict, rowCount_++, columnCount_, bNumCol_);
		pNorRow->SetTable(this);
		pRows_.push_back(pNorRow);
		return pNorRow;
	}

	CPMultiRow* CPTableBase::AddMultiRow()
	{
		CPMultiRow* pMulRow = new CPMultiRow(_pTableDict, rowCount_++, columnCount_, bNumCol_);
		pMulRow->SetTable(this);
		pRows_.push_back(pMulRow);
		return pMulRow;
	}

	ctemplate::TemplateDictionary* CPTableBase::CreateRowDict(ctemplate::TemplateDictionary* pTableDict)
	{
		TemplateDictionary* pRowDict = pTableDict->AddSectionDictionary("ROW");
		return pRowDict;
	}

	void CPTableBase::addHeadCellProperty(ctemplate::TemplateDictionary* pCellDict,
		const std::string& text)
	{
		TemplateDictionary* pCellProDict = pCellDict->AddSectionDictionary("HD_PROPERTY");
		pCellProDict->SetValue("PVALUE", text);
	}

	size_t CPTableBase::rowCount()
	{
		return rowCount_;
	}
}