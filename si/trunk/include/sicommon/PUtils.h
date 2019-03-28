#pragma once

class CPUtils
{
public:
    //mac: .app的父目录
    static std::wstring getAppDir(); //应用程序目录
#ifdef __APPLE__
    //mac: .app目录
    static std::string getMacAppDir();
#endif
    static std::wstring getAppRootDir(); //应用程序上一层的目录
	static std::wstring getGuid();
	//把整数大小改为有单位的，如1024 转换为 1KB
	//2,158,408 转换为2.05MB
	static std::wstring formatSizeUnit(uint64 size);
	//2158408 转换为2,158,408
	static std::wstring formatSizeComma(uint64 size);
	static std::wstring formatSizeUnitComma(uint64 size);
	static std::wstring getCurrentTimeString();
	//转换为时间段，如09:30:45
	static std::string formatTimePeriod(size_t period);
    static bool getFileContent(const std::string& path, std::string &content);
};
