#ifndef SI_PASSWORD_PASSWORDTYPE_H
#define SI_PASSWORD_PASSWORDTYPE_H

namespace password
{
	enum PasswordType
	{
		Pswd_None,			//��Ч����
		Pswd_UserAccount,	//Windows�û��˻�
		Pswd_Browser,		//�����
		Pswd_Mail,			//�ʼ�
		Pswd_Bullet,		//���������
		Pswd_Messenger,		//��ʱͨѶ
		Pswd_OutlookPst,	//OutlookPst�ļ�
		Pswd_VNC,			//VNC���
		Pswd_Wireless,		//����
		Pswd_DialupVPN,		//����/VPN
		Pswd_MAX
	};

	//���������ͺ�tablename��nirsoft�������ƶ�Ӧ����
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
		std::map<PasswordType, std::wstring> typeToolName_; //������ȡ���ͺ͹�������
		static CPswdTypeTablename* instance_;
	};
}

#endif