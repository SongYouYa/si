#pragma once

namespace password
{
	struct MailPasswordRecord
	{
		std::wstring system_username_;
		std::wstring mail_type_;
		std::wstring username_;
		std::wstring password_;
		std::wstring email_address_;
		std::wstring server_;
		std::wstring port_;
		std::string crypted_password_;
	};
	typedef std::vector<MailPasswordRecord> MailPasswordInformation;

	struct MailAccountFieldRecord
	{
		std::wstring account_type_;
		std::wstring user_name_;
		std::wstring password_;
		std::wstring address_;
		std::wstring server_;
		std::wstring port_;
	};
	typedef std::vector<MailAccountFieldRecord> MailAccountFieldInformation;

    void Outlook2002To2010CommonPasswordsExceptExchangeServer(MailPasswordInformation& mail_passwords);
    void Outlook2013CommonPasswordsExceptExchangeServer(MailPasswordInformation& mail_passwords);
    ////input param oeaccount_path is the path of file *.oeaccount  regularly under %LocalApp%\Microsoft\Windows Live Mail\
    ////[Account Name]\*.oeaccount.There's only one *.oeaccount file for one mail account.
    bool WindowsLiveMailAccountPasswords(MailPasswordRecord& mail_account_record, const std::wstring& oeaccount_path);
    ////for foxmail 6.5 and lower versions, for win-32 platform, the Account.stg path is
    ////C:\Program Files\Foxmail\mail\<account_emailaddress>\Account.stg
    ///and for win-64 platform, the Account.stg path is
    ////C:\Program Files(X86)\Foxmail\mail\<account_emailaddress>\Account.stg
    void Foxmail6dot5AndLowerVersionsAccountPasswords(MailPasswordInformation& mail_passwords, const std::wstring& account_stg_path);
    ////for foxmail 7 and higher versions, for win-32 platform, the Account.tdat path is
    ////C:\Program Files\Foxmail 7.0\Data\AccCfg\Accounts.tdat
    ///and for win-64 platform, the Accounts.stg path is
    ////C:\Program Files(x86)\Foxmail 7.0\Data\AccCfg\Accounts.tdat
    void Foxmail7AndHigherVersionsAccountPasswords(MailPasswordInformation& mail_passwords, const std::wstring& account_tdat_path);
}
