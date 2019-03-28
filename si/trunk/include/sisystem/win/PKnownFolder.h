#pragma once

namespace psystem
{
	class SYSTEM_API CPKnownFolder
	{
		class Impl;
	public:
		CPKnownFolder();
		~CPKnownFolder();

		bool init();
		//通过GUID字符串获取代表的windows目录的实际路径
		std::wstring getRealPath(const std::wstring& strGuid);

	private:
		Impl* _impl;
	};
}
