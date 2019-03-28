#pragma once
#include "report/globals.h"

namespace report
{
	//设置报告配置文件主目录
	extern REPORT_API void SetConfRootDir(const std::wstring& dir);

	//设置语言，默认为中文
	extern REPORT_API void SetLanguage(LANGUAGE language); 

	//设置报告头部的名字（Safeimager）
	extern REPORT_API void SetProductName(const std::wstring& productName);

	//高级功能

	//设置主界面中预览表格的行数，默认为10
	extern REPORT_API void SetTablePreviewCount(uint count);
}
