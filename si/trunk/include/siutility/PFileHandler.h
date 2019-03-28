#pragma once
#include "./globals.h"

namespace siutil
{
	class SI_UTILITY_API CPFileHandler
	{
	public:
		//把指定文件的文件打包为一个zip文件
		static bool package(const std::wstring& sourceDir, const std::wstring& targetZip);

		//压缩包默认以“123”为密码
		static bool unzip(const std::wstring& zipPath, const std::wstring& outDir);
	};
}