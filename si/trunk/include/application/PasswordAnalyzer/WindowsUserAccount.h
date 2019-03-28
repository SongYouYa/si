#ifndef SI_PASSWORD_WINDOWSUSERACCOUNT_H
#define SI_PASSWORD_WINDOWSUSERACCOUNT_H
#include "./globals.h"

namespace password
{
	struct WindowsAccountInfo
	{
		std::wstring user_name_;
		std::wstring lm_pass_; //密码
		std::wstring package_; //安全协议
		std::wstring domain_; //域名
		std::wstring lm_hash_;
		std::wstring nt_hash_;
	};
	typedef std::vector<WindowsAccountInfo>  SAMPasswordInformation;

	class PASS_API CWindowsUserAccountPassword
	{
	public:
		static bool GetWindowsUserAccountInformation(CPRecord* parentRecord, CPTaskStatusCtrl* statusCtrl);

	private:
		static bool ParseToSAMPasswordInformation(std::wstring cmd_path, CPRecord* parentRecord,
			CPTaskStatusCtrl* statusCtrl);
	};
}

#endif