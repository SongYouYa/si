#include "stdafx.h"
#include "PRegParserUtils.h"


namespace app
{
	std::wstring CPRegParserUtils::getSystemCurrentSetName(CPRegFile* regFile)
	{
		SIPKEY key;
		std::wstring keyPath = L"Select";
		if (!CPRegistryParser::openKey(regFile, keyPath, key)) {
			return L"";
		}

		CPRegistryValue regValue;
		if (!CPRegistryParser::getValue(key, L"Current", &regValue)) {
			return L"";
		}
		uint curNum = regValue.toUint();

        const int BUF_LEN = 4;
        wchar_t strBuf[BUF_LEN] = { 0 };
#ifdef _MSC_VER
		wsprintf(strBuf, L"%03d", curNum);
#elif __linux__
        swprintf(strBuf, BUF_LEN, L"%03d", curNum);
#endif

		return std::wstring(L"ControlSet") + strBuf;
	}
}
