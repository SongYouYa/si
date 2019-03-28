#pragma once


class CPProcess
{
public:
	//由于要隐藏cmd执行窗口
	static bool createProcess(const std::wstring& cmd,
        const std::wstring& workDir = L"",
		std::string* pOutput = NULL,
        int* pExitCode = NULL);

	//把输出定向到指定文件(输出数据较多时，把输出导出到文件比直接获取输出要快很多)
	static bool createProcess(const std::wstring& cmd,
		const std::wstring& workDir,
		const std::wstring& outFile,
        int* pExitCode = NULL);
};
