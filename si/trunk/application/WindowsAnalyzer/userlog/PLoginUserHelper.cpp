#include "stdafx.h"
#include "PLoginUserHelper.h"
#include "Win32WMIHelper.h"

#include <Wbemidl.h> 

#pragma comment(lib, "wbemuuid.lib")

namespace app
{
	bool CPLoginUserHelper::GetAllUserName(std::vector<std::wstring>& usernames)
	{
		std::vector<Win32UserAccount> user_accounts;

		if (Win32WMIHelper::GetInstance().GetUserAccountsInformation(user_accounts))
		{
			for (size_t i = 0; i < user_accounts.size(); ++i)
			{
				Win32UserAccount user_account = user_accounts.at(i);
				usernames.push_back(user_account.name_.c_str());
			}

			return true;
		}
		return false;
	}
	bool CPLoginUserHelper::GetLogonUsersInformation(LogonUsersInformation& logon_users)
	{
		std::vector<Win32LogonAccount> loginuser_accounts;
		LogonUserInformation logon_user;

		if (Win32WMIHelper::GetInstance().GetLogonUserAccountsInformation(loginuser_accounts))
		{
			int totalSize = loginuser_accounts.size();
			for (size_t i = 0; i < loginuser_accounts.size(); ++i)
			{
				Win32LogonAccount user_account = loginuser_accounts.at(i);
				logon_user.user_caption_ = user_account.caption_;
				logon_user.user_login_time_ = CPTime::UTC2LocalTime(user_account.start_time_, CPTime::GetCurrentTimeZoneBias());
				logon_users.push_back(logon_user);
			}

			return true;
		}
		return false;
	}
	std::wstring CPLoginUserHelper::GetUserSID(const std::wstring& user_name)
	{
		std::wstring user_sid;
		std::vector<Win32UserAccount> user_accounts;
		Win32WMIHelper::GetInstance().GetUserAccountsInformation(user_accounts);

		for (size_t i = 0; i < user_accounts.size(); ++i)
		{
			Win32UserAccount user_account = user_accounts.at(i);

			if (user_account.name_ == user_name)
			{
				user_sid.clear();
				user_sid = user_account.sid_.c_str();
				return user_sid;
			}
		}

		return L"";
	}

	bool CPLoginUserHelper::analyze(CPRecord* parentRecord)
	{
		std::vector<LogonUserInformation> logonUsersinfo;
		GetLogonUsersInformation(logonUsersinfo);
		if (logonUsersinfo.empty()) {
			return false;
		}

		for (auto it = logonUsersinfo.begin(); it != logonUsersinfo.end(); ++it) {
			LogonUserInformation& logonUsersinfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"Caption") = logonUsersinfo.user_caption_;
			record->at(L"Time") = logonUsersinfo.user_login_time_;
		}
		return true;
	}
}
