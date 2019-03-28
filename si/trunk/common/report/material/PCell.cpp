#include "stdafx.h"
#include "PCell.h"
#include "PRow.h"
#include "PTable.h"
#include <cstdio>

using namespace ctemplate;

const std::string CELL_DATA = "CELL_DATA";

namespace report
{
	CPCell::CPCell(ctemplate::TemplateDictionary* pCellDict, CPRow* pRow)
		: pCellDict_(pCellDict), pColSpanDict_(NULL), colSpan_(1), pRow_(pRow)
	{

	}

	CPCell::~CPCell()
	{

	}


	CPRow* CPCell::Row()
	{
		return pRow_;
	}

    void CPCell::SetData(const std::string& strData)
	{
		pCellDict_->SetValue(CELL_DATA, strData);
	}

	void CPCell::SetData(uint64 value)
	{
		std::ostringstream ss;
		ss << value;
		SetData(ss.str());
	}

	void CPCell::SetData(int64 value)
	{
		std::ostringstream ss;
		ss << value;
		SetData(ss.str());
	}

	void CPCell::SetData(uint value)
	{
		std::ostringstream ss;
		ss << value;
		SetData(ss.str());
	}

	void CPCell::SetData(int value)
	{
		std::ostringstream ss;
		ss << value;
		SetData(ss.str());
	}

	void CPCell::SetData(float f)
	{
		char buf[40] = { 0 };
        psprintfs(buf, sizeof(buf), "%f", f);
		SetData(buf);
	}

	void CPCell::SetData(double db)
	{
        char buf[40] = {0};
        psprintfs(buf, sizeof(buf), "%lf", db);
		SetData(buf);
	}

	void CPCell::SetRowSpan(int spanCount)
	{
		TemplateDictionary* pRowSpanDict = pCellDict_->AddSectionDictionary("ROW_SPAN");
		pRowSpanDict->SetIntValue("SPAN_COUNT", spanCount);
	}

	void CPCell::SetColSpan(int spanCount)
	{
		//列跨度为1时，不必设
		if (spanCount <= 1)
		{
			return;
		}

		if (pColSpanDict_ == NULL)
		{
			pColSpanDict_ = pCellDict_->AddSectionDictionary("COL_SPAN");
		}
		pColSpanDict_->SetIntValue("SPAN_COUNT", spanCount);
		colSpan_ = spanCount;
	}

	void CPCell::SetImg(const std::string& strSrc)
	{
		if (!strSrc.empty())
		{
			TemplateDictionary* pImgDict = pCellDict_->AddSectionDictionary("IMG");
			//std::string upperDir = Row()->Table()->Html()->UpperDirName();
			//pImgDict->SetValue("SRC", upperDir + strSrc);
		}
	}

	void CPCell::SetArchor(const std::string& archorName, const std::string& archorText)
	{
		TemplateDictionary* pArchorDict = pCellDict_->AddSectionDictionary("ARCHOR");
		pArchorDict->SetValue("TARGET_NAME", archorName);
		pArchorDict->SetValue("TEXT", archorText);
	}

	void CPCell::SetClass(const std::string& strClass)
	{
		TemplateDictionary* pClassDict = pCellDict_->AddSectionDictionary("CLASS");
		pClassDict->SetValue("NAME", strClass);
	}

	int CPCell::GetColSpan()
	{
		return colSpan_;
	}

	void CPCell::SetUrl(const std::string& urlAdd, const std::string& urlDisplay)
	{
		TemplateDictionary* pUrlDict = pCellDict_->AddSectionDictionary("URL");
		pUrlDict->SetValue("URL_ADD", urlAdd);
		pUrlDict->SetValue("URL_DISPLAY", urlDisplay);
	}

	void CPCell::SetMediaImg(const std::string& strSrc)
	{
		if (!strSrc.empty()) {
			TemplateDictionary* pMediaImgDict = pCellDict_->AddSectionDictionary("MEDIA_IMG");
			pMediaImgDict->SetValue("IMG_PATH", strSrc);
		}
	}

	void CPCell::SetMediaCommon(const std::string& strPath, const std::string& mediaName)
	{
		TemplateDictionary* pMediaCommonDict = pCellDict_->AddSectionDictionary("MEDIA_COMMON");
		pMediaCommonDict->SetValue("MEDIA_PATH", strPath);
		pMediaCommonDict->SetValue("MEDIA_NAME", mediaName);
	}

}
