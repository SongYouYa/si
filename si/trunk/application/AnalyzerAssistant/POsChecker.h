#pragma once


namespace app
{
#if 0
	enum OsType
	{
		OS_UNKNOWN,
		OS_WIN,
		OS_MAC
	};
	struct OsInfo
	{
		OsType osType;
		bool online;
		OsInfo()
			: osType(OS_UNKNOWN), online(false)
		{}
	};
	typedef std::vector<OsInfo> OsInfos;
#endif

	//主要用于离线环境下，检测本地操作系统类型
	class CPOsChecker
	{
	public:
#if 0
		static void getOses(OsInfos& osInfos);
#endif
		CPOsChecker();
		bool hasMacOs();
		bool hasWinOs();

	private:
		void init();

		bool _hasMac;
		bool _hasWin;
	};
}