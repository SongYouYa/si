#include "stdafx.h"
#include "material/PPageJumper.h"
#include "material/PCssDefine.h"

namespace report
{
    CPPageJumper::CPPageJumper()
	{

	}

	CPPageJumper::~CPPageJumper()
	{

	}

	void CPPageJumper::getIndexFilenames(size_t recordCount, size_t unitCount,
		std::map<uint, std::wstring>& index_filenames)
	{
		size_t pageTatalCount = (recordCount % unitCount) ? (recordCount / unitCount + 1) : (recordCount / unitCount);

		std::wstring indexHtmlName;
		for (size_t i = 1; i <= pageTatalCount; ++i)
		{
			std::wstring htmlFileName = L"index";
			if (i != 1) {
				htmlFileName += L"-" + I2WS(i);
			}
			htmlFileName += L".html";
			index_filenames[i] = htmlFileName;
		}
	}

	void CPPageJumper::setPageJump(TemplateDictionary* pageJumpDict,
		const std::map<uint, std::wstring>& index_FileNames,
		int currentIndex)
	{
		//根据中英文,设置显示文本
		pageJumpDict->SetValue("TOTAL_PAGE_TEXT", GET_TEXT(TEXT_ID_TOTAL_PAGE));

		//设置数量
		pageJumpDict->SetIntValue("TOTAL_PAGE_COUNT", index_FileNames.size());

		//设置下拉框
		for (auto it = index_FileNames.begin(); it != index_FileNames.end(); ++it)
		{
			int ioption = it->first;
			TemplateDictionary* pOptionDict = pageJumpDict->AddSectionDictionary("OPTION");
			pOptionDict->SetIntValue("OPTION_NUM", ioption);
			std::wstring targetHtml = it->second;
			pOptionDict->SetValue("VALUE", W2U8(targetHtml));
			if (ioption == currentIndex)
			{
				pOptionDict->AddSectionDictionary("SELECTED");
			}
		}

		std::wstring previseHtmlFile;
		std::wstring laterHtmlFile;
		std::vector<std::pair<int, std::wstring>> beside_index_FileNames;
		getBesidePages(index_FileNames, currentIndex, beside_index_FileNames, previseHtmlFile, laterHtmlFile);

		const std::wstring PREVISE_ARCHOR_TEXT = L"<";
		const std::wstring LATER_ARCHOR_TEXT = L">";

		//创建上一页锚接
		TemplateDictionary* pPreviseArchorDict = createPageArchorDict(pageJumpDict, W2U8(PREVISE_ARCHOR_TEXT));
		if (!previseHtmlFile.empty())
		{
			setPageArchorHref(pPreviseArchorDict, W2U8(previseHtmlFile));
		}

		//创建页码锚接
		for (auto it = beside_index_FileNames.begin(); it != beside_index_FileNames.end(); ++it)
		{
			int index = (*it).first;
			TemplateDictionary* pPageArchorDict = createPageArchorDict(pageJumpDict, I2S(index));
			if (index == currentIndex)
			{
				setPageArchorClass(pPageArchorDict, CLASS_PAGE_CHECKED);
			}
			else
			{
				std::wstring wstrHtmlFile = (*it).second;
				setPageArchorHref(pPageArchorDict, W2U8(wstrHtmlFile));
			}
		}

		//创建下一页锚接
		TemplateDictionary* pLaterArchorDict = createPageArchorDict(pageJumpDict, W2U8(LATER_ARCHOR_TEXT));
		if (!laterHtmlFile.empty())
		{
			setPageArchorHref(pLaterArchorDict, W2U8(laterHtmlFile));
		}
	}

	void CPPageJumper::getBesidePages(const std::map<uint, std::wstring>& index_FileNames,
		int currentIndex,
		std::vector<std::pair<int, std::wstring>>& beside_index_FileNames,
		std::wstring& previseHtmlFile, std::wstring& laterHtmlFile)
	{
		const int INDEX_ARCHOR_COUNT = 5;
		const int HALF_ARCHOR_COUNT = INDEX_ARCHOR_COUNT / 2;

		//计算开始和结束索引
		int tempMin = currentIndex - HALF_ARCHOR_COUNT;
		int indexMin = tempMin < 1 ? 1 : tempMin;
		size_t tempMax = indexMin + INDEX_ARCHOR_COUNT - 1;
		int indexMax = tempMax > index_FileNames.size() ? index_FileNames.size() : tempMax;
		indexMin -= INDEX_ARCHOR_COUNT - (indexMax - indexMin + 1);
		indexMin = indexMin < 1 ? 1 : indexMin;

		for (int i = indexMin; i <= indexMax; ++i)
		{
			auto it_i = index_FileNames.find(i);
			if (it_i != index_FileNames.end())
			{
				beside_index_FileNames.push_back(*it_i);
			}
		}

		int previseIndex = currentIndex - 1;
		int laterIndex = currentIndex + 1;
		if (index_FileNames.find(previseIndex) != index_FileNames.end())
		{
			previseHtmlFile = index_FileNames.at(previseIndex);
		}
		if (index_FileNames.find(laterIndex) != index_FileNames.end())
		{
			laterHtmlFile = index_FileNames.at(laterIndex);
		}
	}

	TemplateDictionary* CPPageJumper::createPageArchorDict(TemplateDictionary* pPageJumpDict, const std::string& archorText)
	{
		TemplateDictionary* pPageArchorDict = pPageJumpDict->AddSectionDictionary("PAGE_ARCHOR");
		pPageArchorDict->SetValue("TARGET_TEXT", archorText);
		return pPageArchorDict;
	}

	void CPPageJumper::setPageArchorHref(ctemplate::TemplateDictionary* pPageArchorDict, const std::string& strHref)
	{
		TemplateDictionary* pHrefDict = pPageArchorDict->AddSectionDictionary("HREF");
		pHrefDict->SetValue("TARGET_HTML", strHref);
	}

	void CPPageJumper::setPageArchorClass(ctemplate::TemplateDictionary* pPageArchorDict, const std::string& strClass)
	{
		TemplateDictionary* pClassDict = pPageArchorDict->AddSectionDictionary("CLASS");
		pClassDict->SetValue("CLASS_NAME", strClass);
	}
}
