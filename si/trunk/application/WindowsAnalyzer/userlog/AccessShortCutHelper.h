#pragma once
namespace app
{
	class AccessShortcutHelper
	{
	public:
		struct AccessShortcut
		{
			std::wstring link_path_;	//快捷方式路径
			std::wstring access_time_;	//访问时间
		};
		typedef std::vector<AccessShortcut> vecAccessShortcutInformation;

		static bool GetAccessShortcuts(vecAccessShortcutInformation &vecInformation);
		static bool analyze(CPRecord* rootRecord);
		static void ROT13Ex(TCHAR * tszBuffer, DWORD dwLen); //对“凯撒13”加密算法解密
		AccessShortcutHelper(void);
		~AccessShortcutHelper(void);
	};
}
