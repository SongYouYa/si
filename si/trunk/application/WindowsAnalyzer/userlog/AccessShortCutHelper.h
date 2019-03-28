#pragma once
namespace app
{
	class AccessShortcutHelper
	{
	public:
		struct AccessShortcut
		{
			std::wstring link_path_;	//��ݷ�ʽ·��
			std::wstring access_time_;	//����ʱ��
		};
		typedef std::vector<AccessShortcut> vecAccessShortcutInformation;

		static bool GetAccessShortcuts(vecAccessShortcutInformation &vecInformation);
		static bool analyze(CPRecord* rootRecord);
		static void ROT13Ex(TCHAR * tszBuffer, DWORD dwLen); //�ԡ�����13�������㷨����
		AccessShortcutHelper(void);
		~AccessShortcutHelper(void);
	};
}
