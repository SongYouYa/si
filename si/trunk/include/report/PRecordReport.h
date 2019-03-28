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

		//�ɹ����أ�html·��
		//ʧ��,���ؿ�
		std::wstring createReport(const std::wstring& outDir, bool withMedia);

	private:
		void createContentPages(CPRecord* parentRecord, const std::wstring& outDir, int level);

		CPRecord* _rootRecord;
		std::wstring _rootDir;
		bool _withMedia;
	};
}
