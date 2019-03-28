#ifndef SI_PASSWORD_PASSWORDTYPE_H
#define SI_PASSWORD_PASSWORDTYPE_H

namespace password
{
	enum PasswordType
	{
		Pswd_None,			//无效类型
		Pswd_UserAccount,	//Windows用户账户
		Pswd_Browser,		//浏览器
		Pswd_Mail,			//邮件
		Pswd_Bullet,		//密码输入框
		Pswd_Messenger,		//即时通讯
		Pswd_OutlookPst,	//OutlookPst文件
		Pswd_VNC,			//VNC软件
		Pswd_Wireless,		//无线
		Pswd_DialupVPN,		//拨号/VPN
		Pswd_MAX
	};

	//把密码类型和tablename、nirsoft工具名称对应起来
	class CPswdTypeTablename
	{
	public:
		static CPswdTypeTablename* Instance();

		std::wstring GetTableName(PasswordType type);
		PasswordType GetPsType(const std::wstring& tableName);
		std::wstring GetToolName(PasswordType type);

	private:
		CPswdTypeTablename();
		~CPswdTypeTablename();

		std::map<PasswordType, std::wstring> typeTablename_;
		std::map<PasswordType, std::wstring> typeToolName_; //密码提取类型和工具名称
		static CPswdTypeTablename* instance_;
	};
}

#endif