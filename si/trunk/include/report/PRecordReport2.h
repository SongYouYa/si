#pragma once
#include "report/globals.h"

namespace report
{
	class REPORT_API CPRecordReport2
	{
	public:
		CPRecordReport2(CPRecord* rootRecord);

		//成功返回，html路径
		//失败,返回空
		std::wstring createReport(const std::wstring& outDir);

	private:
		CPRecord* _rootRecord;
		std::wstring _rootDir;
	};
}