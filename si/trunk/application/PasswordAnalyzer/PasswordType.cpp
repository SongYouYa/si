#include "stdafx.h"
#include "PasswordType.h"

namespace password
{
	CPswdTypeTablename* CPswdTypeTablename::instance_ = NULL;

	CPswdTypeTablename::CPswdTypeTablename()
	{
		//配置文件中tablename
		typeTablename_[Pswd_Browser] = L"table.win.BrowserPassword";
		typeTablename_[Pswd_Mail] = L"table.win.MailPassword";
		typeTablename_[Pswd_Bullet] = L"table.win.BulletPassword";
		typeTablename_[Pswd_Messenger] = L"table.win.IMPassword";
		typeTablename_[Pswd_OutlookPst] = L"table.win.OutlookPstPassword";
		typeTablename_[Pswd_VNC] = L"table.win.VNCPassword";
		typeTablename_[Pswd_Wireless] = L"table.win.WirelessPassword";
		typeTablename_[Pswd_DialupVPN] = L"table.win.DialupVPNPassword";

		//nirsoft工具名称
		typeToolName_[Pswd_Browser] = L"WebBrowserPassView";
		typeToolName_[Pswd_Mail] = L"mailpv";
		typeToolName_[Pswd_Bullet] = L"BulletsPassView";
		typeToolName_[Pswd_Messenger] = L"mspass";
		typeToolName_[Pswd_OutlookPst] = L"PstPassword";
		typeToolName_[Pswd_VNC] = L"VNCPassView";
		typeToolName_[Pswd_Wireless] = L"WirelessKeyView";
		typeToolName_[Pswd_DialupVPN] = L"Dialupass";
	}

	CPswdTypeTablename::~CPswdTypeTablename()
	{
			
	}

	CPswdTypeTablename* CPswdTypeTablename::Instance()
	{
		if (!instance_)
		{
			instance_ = new CPswdTypeTablename;
		}

		return instance_;
	}

	std::wstring CPswdTypeTablename::GetTableName( PasswordType type )
	{
		auto it = typeTablename_.find(type);
		if (it != typeTablename_.end())
		{
			return typeTablename_.at(type);
		}

		return L"";
	}

	PasswordType CPswdTypeTablename::GetPsType(const std::wstring& tableName)
	{
		for (auto it = typeTablename_.begin(); it != typeTablename_.end(); ++it)
		{
			std::wstring name = it->second;
			if (name == tableName)
			{
				return it->first;
			}
		}

		return PasswordType::Pswd_None;
	}

	std::wstring CPswdTypeTablename::GetToolName( PasswordType type )
	{
		auto it = typeToolName_.find(type);
		if (it != typeToolName_.end())
		{
			return typeToolName_.at(type);
		}

		return L"";
	}

}