#pragma once
namespace app
{
	class LogonListInfoHelper //:public MemPipeParser
	{
	public:
		enum LogonListData
		{
			USERNAME = 0,
			COMPUTER,
			USERDOMIAN,
			LOGONTIME,
			LOGOFFTIME,
			DURATION
		};
		typedef std::vector<std::wstring> UserInformation;
		typedef std::vector<UserInformation> vecLogonListInformation;
		LogonListInfoHelper(void);
		~LogonListInfoHelper(void);
		static bool analyze(CPRecord *rootRecord);
		
	private:
		static bool ParseLogonUserListInformation(vecLogonListInformation &vecLogonUsers);
		static bool ParseXpUsers(vecLogonListInformation &vecInformaion, const std::wstring& cmd);
		static bool GetVistaLaterLogonUsers(vecLogonListInformation &vecLogonUsers);
		static bool GetXPLogonUsers(vecLogonListInformation &vecLogonUsers);
		static bool ParseListXml(const std::wstring& wstrXmlPath,vecLogonListInformation &vecLogonUsers);
	};
}



