#pragma once

#include "report/globals.h"

namespace report
{
	class REPORT_API CPReportUtility
	{
	public:
		//��Ĭ���������html
		static bool OpenHtml(const std::wstring& htmlPath);
	};
}