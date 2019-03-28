#pragma once
#include "./globals.h"

namespace siutil
{
	class SI_UTILITY_API CPFileHandler
	{
	public:
		//��ָ���ļ����ļ����Ϊһ��zip�ļ�
		static bool package(const std::wstring& sourceDir, const std::wstring& targetZip);

		//ѹ����Ĭ���ԡ�123��Ϊ����
		static bool unzip(const std::wstring& zipPath, const std::wstring& outDir);
	};
}