#pragma once
#include "report/globals.h"

namespace report
{
	class REPORT_API CPRecordReport2
	{
	public:
		CPRecordReport2(CPRecord* rootRecord);

		//�ɹ����أ�html·��
		//ʧ��,���ؿ�
		std::wstring createReport(const std::wstring& outDir);

	private:
		CPRecord* _rootRecord;
		std::wstring _rootDir;
	};
}