#pragma once


namespace net
{
	class CPShareFolder
	{
	public:
#ifdef _MSC_VER
		static bool openShareFolder(const std::wstring& remoteDir,
			const std::wstring& userName, const std::wstring& password);
#elif __GNUC__
		static bool mountShareFolder(const std::wstring& remoteDir,
			const std::wstring& userName, const std::wstring& password,
			const std::wstring& localDir);
#endif
	};
}
