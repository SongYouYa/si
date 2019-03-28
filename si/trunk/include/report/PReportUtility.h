#pragma once

#include "report/globals.h"

namespace report
{
	class REPORT_API CPReportUtility
	{
	public:
		//用默认浏览器打开html
		static bool OpenHtml(const std::wstring& htmlPath);
	};
}