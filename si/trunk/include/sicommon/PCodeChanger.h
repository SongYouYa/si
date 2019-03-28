#pragma once

class CPCodeChanger
{
	class Impl;
public:
	static std::string UnicodeToUTF8(const std::wstring& wstr);
	static std::wstring UTF8ToUnicode(const std::string& str);

    //windows�����£������·����һ��תΪmbcs(GBK)������תΪutf8
	static std::string UnicodeToGB(const std::wstring& wstr);
	static std::wstring GBToUnicode(const std::string& str);

    //��UTF16LE�Ķ���������ת��Ϊ���ַ���
    static std::wstring UTF16LEToWString(const void* data, size_t length);
};

inline std::string W2U8(const std::wstring& wstr)
{
	return CPCodeChanger::UnicodeToUTF8(wstr);
}

inline std::wstring U82W(const std::string& str)
{
	return CPCodeChanger::UTF8ToUnicode(str);
}

inline std::wstring LS2W(const std::string& str)
{
#ifdef _MSC_VER
	return CPCodeChanger::GBToUnicode(str);
#else
	return CPCodeChanger::UTF8ToUnicode(str);
#endif
}

inline std::string W2LS(const std::wstring& wstr)
{
#ifdef _MSC_VER
	return CPCodeChanger::UnicodeToGB(wstr);
#else
	return CPCodeChanger::UnicodeToUTF8(wstr);
#endif
}

inline std::wstring GB2W(const std::string& str)
{
	return CPCodeChanger::GBToUnicode(str);
}

inline std::string W2GB(const std::wstring& wstr)
{
	return CPCodeChanger::UnicodeToGB(wstr);
}
