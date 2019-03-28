#include "stdafx.h"
#ifndef __clang__
#pragma execution_character_set("utf-8")
#endif

#include "PTable.h"
#include "PCssDefine.h"


using namespace ctemplate;


const std::string HEAD_CLASS = "HEAD_CLASS";
const std::string HEAD_CELL_CLASS_NAME = "NAME";

const std::string BASE_ARCHOR_NAME = "Archor_";
const std::string BASE_ID = "_ID_";

namespace report
{

	CPTable::CPTable(TemplateDictionary* pTableDict, uint columnCount, CPPage* page, CPTable* pParent, size_t rowBeginIndex, bool bNumCol)
		: CPTableBase(pTableDict, columnCount, bNumCol),
		level_(0), pParent_(pParent), index_(0), _page(page)
	{
		rowCount_ = rowBeginIndex;
		archorName_ = BASE_ARCHOR_NAME + I2S(++s_archorNumber);
		
		if (pParent == NULL)
		{
			index_ = 1;
			static int rootTableNum = 0;
			++rootTableNum;
			idStr_ = "_" + I2S(rootTableNum) + BASE_ID;
		}
		else
		{
			level_ = pParent->Level() + 1;
			index_ = pParent->rowCount() + 1;
			pParent->AddChild(this);

			//设置table的ID，每个table有唯一的ID
			//设置的顺序：parent的ID在最后，方便css定位
			idStr_ = "_" + UI2S(index_) +"_" + pParent->IdStr();
		}
		_pTableDict->SetValue("ID", idStr_);
	}

	CPTable::~CPTable()
	{
		
	}

	std::string CPTable::archorName()
	{
		return archorName_;
	}

	void CPTable::addTitle(const std::string& imgSrc, const std::string& text)
	{
		TemplateDictionary* pTitleDict = _pTableDict->AddSectionDictionary("TITLE");
		if (!imgSrc.empty())
		{
			std::string updirName = "";// pHtml_->UpperDirName();
			TemplateDictionary* pImgDict = pTitleDict->AddSectionDictionary("IMG");
			pImgDict->SetValue("SRC", updirName + imgSrc);
		}

		std::string selfTitleText;
		if (pParent_ && pParent_->Level() > 0)
		{
			std::string parentTitle = pParent_->title();
			std::string parentArchor = pParent_->archorName();
			TemplateDictionary* parentArchorDict = pTitleDict->AddSectionDictionary("PARENT_ARCHOR");
			parentArchorDict->SetValue("TARGET_NAME", parentArchor);
			parentArchorDict->SetValue("TARGET_TEXT", parentTitle);
			titleStr_ = pParent_->title() + " > " + text;
			selfTitleText = " > ";
		}
		else
		{
			titleStr_ = text;
		}
		selfTitleText += text;
		pTitleDict->SetValue("TEXT", selfTitleText);

        int level = pmax(level_, 1);
        level = pmin(level, 6); //html的h标签不能大于6
		pTitleDict->SetIntValue("LEVEL", level);
		pTitleDict->SetValue("ARCHOR_NAME", archorName_);

		pTitleDict->SetValue("ID", idStr_);

	}

	void CPTable::AddClass(const std::string& strClass)
	{
		TemplateDictionary* pClassDict = _pTableDict->AddSectionDictionary("TABLE_CLASS");
		pClassDict->SetValue("CLASS_NAME", strClass);
	}

	int CPTable::Level()
	{
		return level_;
	}

	void CPTable::AddChild(CPTable* pChild)
	{
		pChildren_.push_back(pChild);
	}

	std::string CPTable::title()
	{
		return titleStr_;
	}

	std::string CPTable::IdStr()
	{
		return idStr_;
	}

	void CPTable::SetHeadClass(const std::string& strClass)
	{
		if (!_pHeadDict) {
			return;
		}
		_pHeadDict->SetValue(HEAD_CLASS, strClass);
	}

	void CPTable::SetLevel(int level)
	{
		level_ = level;
	}

	CPPage* CPTable::page()
	{
		return _page;
	}

}
