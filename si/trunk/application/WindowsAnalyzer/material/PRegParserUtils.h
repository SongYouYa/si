#pragma once


namespace app
{
	class CPRegParserUtils
	{
	public:
		//system注册表根目录下，没有CurrentControlSet这一项，需要解析出来
		static std::wstring getSystemCurrentSetName(CPRegFile* regFile);
	};
}