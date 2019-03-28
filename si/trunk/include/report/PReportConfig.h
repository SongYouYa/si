#pragma once
#include "report/globals.h"

namespace report
{
	//���ñ��������ļ���Ŀ¼
	extern REPORT_API void SetConfRootDir(const std::wstring& dir);

	//�������ԣ�Ĭ��Ϊ����
	extern REPORT_API void SetLanguage(LANGUAGE language); 

	//���ñ���ͷ�������֣�Safeimager��
	extern REPORT_API void SetProductName(const std::wstring& productName);

	//�߼�����

	//������������Ԥ������������Ĭ��Ϊ10
	extern REPORT_API void SetTablePreviewCount(uint count);
}
