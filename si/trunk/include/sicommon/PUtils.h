#pragma once

class CPUtils
{
public:
    //mac: .app�ĸ�Ŀ¼
    static std::wstring getAppDir(); //Ӧ�ó���Ŀ¼
#ifdef __APPLE__
    //mac: .appĿ¼
    static std::string getMacAppDir();
#endif
    static std::wstring getAppRootDir(); //Ӧ�ó�����һ���Ŀ¼
	static std::wstring getGuid();
	//��������С��Ϊ�е�λ�ģ���1024 ת��Ϊ 1KB
	//2,158,408 ת��Ϊ2.05MB
	static std::wstring formatSizeUnit(uint64 size);
	//2158408 ת��Ϊ2,158,408
	static std::wstring formatSizeComma(uint64 size);
	static std::wstring formatSizeUnitComma(uint64 size);
	static std::wstring getCurrentTimeString();
	//ת��Ϊʱ��Σ���09:30:45
	static std::string formatTimePeriod(size_t period);
    static bool getFileContent(const std::string& path, std::string &content);
};
