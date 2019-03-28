#pragma once

namespace app
{
	class CPLoginUserHelper
    {
	public:
		struct LogonUserInformation
		{
			std::wstring user_caption_;
			CPTime user_login_time_;
		};
		typedef std::vector<LogonUserInformation> LogonUsersInformation;
		static bool analyze(CPRecord* parentRecord);

    private:
        bool GetAllUserName(std::vector<std::wstring>& usernames);
        static bool  GetLogonUsersInformation(LogonUsersInformation& logon_users);   //(std::vector<std::pair<std::wstring, std::wstring>>& logon_users);
        std::wstring GetUserSID(const std::wstring& user_name);
    };
}