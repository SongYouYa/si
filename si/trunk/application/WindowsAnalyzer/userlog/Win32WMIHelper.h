#pragma once
namespace app
{
	struct Win32UserAccount
	{
		uint32 account_type_;//256-
		std::wstring caption_;//LOTUS-VM2\Administrator
		std::wstring description_;
		bool disabled_;
		std::wstring domain_; //name of the windows domain to which a user account belongs, for example "NA-SALES"
		std::wstring full_name_;
		bool local_account_;//
		bool lockout_;
		std::wstring name_;
		bool password_changeable_;
		bool password_expires_;
		bool password_required_;
		std::wstring sid_;
		uint8 sid_type_;
		std::wstring status_;
	};
	struct  Win32LogonAccount
	{
		std::wstring caption_;
		std::wstring description_;
		std::wstring domain_;
		bool local_account_;
		std::wstring name_;
		std::wstring sid_;
		int64 start_time_;
		uint8 sid_type_;
		std::wstring status_;
	};

	class  Win32WMIHelper
	{
	public:
		class Win32WMIHelperImpl;
		typedef boost::shared_ptr<Win32WMIHelperImpl> Win32WMIHelperImplPtr;
		Win32WMIHelper();
		~Win32WMIHelper();
		static Win32WMIHelper& GetInstance();

		// Get local machine's all user accounts
		bool GetUserAccountsInformation(std::vector<Win32UserAccount> & user_accounts);
		// Get Local machine's all login user accounts
		bool GetLogonUserAccountsInformation(std::vector<Win32LogonAccount>&  login_user_accounts);
		bool GetLogonTime(std::wstring logon_id, std::wstring& logon_time);
		time_t ParseDateTimeToTimet(const std::wstring& date_time);
	private:
		Win32WMIHelperImpl *implement_;
	};
}
