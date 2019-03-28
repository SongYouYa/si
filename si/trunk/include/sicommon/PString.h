#pragma once

class CPString
{
	class Impl;
public:
	//string
	static bool findNoCase(const std::wstring& src, const std::wstring& look);
	static bool isEquali(const std::wstring& first, const std::wstring& second);
    static /*explicit*/ bool isEquali(const std::string& first, const std::string& second);
    static bool isEquali(const char* first, const char* second);
	static std::wstring trim(const std::wstring& src, wchar_t ch = L' ');
    static std::string trim(const std::string& src, char ch = ' ');
	static std::wstring getFileName(const std::wstring& path, wchar_t ch = L'\\');
	static std::wstring getFileExt(const std::wstring& path);

	static void split(const std::string& str, char token,
		std::vector<std::string>& tokens);
	static void wsplit(const std::wstring& str, wchar_t token,
		std::vector<std::wstring>& tokens);

	static void joinArray(const std::vector<std::wstring>& strArray,
		const std::wstring& token, std::wstring& out);

	static bool s2i(const std::string& str, int& n);
	static std::wstring i2ws(int64 i);
	static std::wstring i2ws(uint64 i);
	static std::wstring i2ws(int i);
	static std::wstring i2ws(uint i);
};

#define I2WS(i) (std::to_wstring(i))
#define I2S(i) (std::to_string(i))
