#pragma once


namespace app
{
	class CPRegParserUtils
	{
	public:
		//systemע����Ŀ¼�£�û��CurrentControlSet��һ���Ҫ��������
		static std::wstring getSystemCurrentSetName(CPRegFile* regFile);
	};
}