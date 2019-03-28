#pragma once
#include "report/globals.h"

namespace report
{
	class CPTree;
	class REPORT_API CPRecordReport
	{
	public:
		CPRecordReport(CPRecord* rootRecord);
		~CPRecordReport();

		//成功返回，html路径
		//失败,返回空
		std::wstring createReport(const std::wstring& outDir, bool withMedia);

	private:
		void createContentPages(CPRecord* parentRecord, const std::wstring& outDir, int level);

		CPRecord* _rootRecord;
		std::wstring _rootDir;
		bool _withMedia;
	};
}
