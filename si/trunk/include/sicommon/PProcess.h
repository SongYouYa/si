#pragma once


class CPProcess
{
public:
	//����Ҫ����cmdִ�д���
	static bool createProcess(const std::wstring& cmd,
        const std::wstring& workDir = L"",
		std::string* pOutput = NULL,
        int* pExitCode = NULL);

	//���������ָ���ļ�(������ݽ϶�ʱ��������������ļ���ֱ�ӻ�ȡ���Ҫ��ܶ�)
	static bool createProcess(const std::wstring& cmd,
		const std::wstring& workDir,
		const std::wstring& outFile,
        int* pExitCode = NULL);
};
