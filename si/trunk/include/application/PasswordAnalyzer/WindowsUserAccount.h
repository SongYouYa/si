#ifndef SI_PASSWORD_WINDOWSUSERACCOUNT_H
#define SI_PASSWORD_WINDOWSUSERACCOUNT_H
#include "./globals.h"

namespace password
{
	struct WindowsAccountInfo
	{
		std::wstring user_name_;
		std::wstring lm_pass_; //����
		std::wstring package_; //��ȫЭ��
		std::wstring domain_; //����
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