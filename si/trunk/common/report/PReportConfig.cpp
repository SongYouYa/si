#include "stdafx.h"
#include "report/PReportConfig.h"
#include "PReportPath.h"


uint g_tablePreviewCount = 10; //��������Ԥ����ʾ����
std::wstring g_productName;

namespace report
{
	void SetTablePreviewCount(uint count)
	{
		g_tablePreviewCount = count;
	}

	void SetConfRootDir(const std::wstring& dir)
	{
		CPReportPath::SetConfRootDir(dir);
	}

	void SetLanguage(LANGUAGE language)
	{
		CPLanguageSource::setLang(language);
	}

	void SetProductName(const std::wstring& productName)
	{
		g_productName = productName;
	}
}