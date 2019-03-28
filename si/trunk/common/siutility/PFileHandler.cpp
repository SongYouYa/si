#include "stdafx.h"
#include "siutility/PFileHandler.h"
#include "siutility/PDirHelper.h"

#include "sicommon/PProcess.h"

namespace siutil
{
	std::wstring quoteParam(const std::wstring& param)
	{
		std::wstring np = L"\"";
		np += param + L"\"";
		return np;
	}


	bool CPFileHandler::package(const std::wstring& sourceDir, const std::wstring& targetZip)
	{
		std::wstring toolDir = CPDirHelper::get()->getToolDir();
#ifdef __linux__
		std::wstring zipTool = toolsDir + L"/zip/7za";
#else
		std::wstring zipTool = toolDir + L"/zip/7z.exe";
#endif
		std::wstring cmd = quoteParam(zipTool) + L" a " + quoteParam(targetZip) + L" " + quoteParam(sourceDir);

		int exitCode = 0;
		if (!CPProcess::createProcess(cmd, L"", NULL, &exitCode) ||
			exitCode != 0) {
			return false;
		}
		return true;
	}

	bool CPFileHandler::unzip(const std::wstring& zipPath, const std::wstring& outDir)
	{
		std::wstring toolDir = siutil::CPDirHelper::get()->getToolDir();
		std::wstring exePath = toolDir + L"\\zip\\7z.exe";

		std::wstring cmd = L"\"" + exePath + L"\" -p123 x \"" + zipPath + L"\" -o\"" + outDir + L"\" -aos";
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd, L"", NULL, &exitCode) ||
			exitCode != 0) {
			return false;
		}
		return true;
	}
}
