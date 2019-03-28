#pragma once

namespace psystem
{
	class SYSTEM_API CPLnkParser
	{
	public:
		static bool parseLnk(const std::wstring& lnkPath, std::wstring& realPath,
			time64_t* modifyTime = nullptr);
	};
}
