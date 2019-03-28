#ifndef SI_PASSWORD_CRACKPASSWORD_H
#define SI_PASSWORD_CRACKPASSWORD_H

#include <QtXml/QDomDocument>
#include "PasswordType.h"

namespace password
{
	class CNirsoftPassword
	{
	public:
		CNirsoftPassword();
		~CNirsoftPassword();

		bool CrackNirsoftPassword(const std::wstring& typeName, CPRecord* parentRecord,
			CPTaskStatusCtrl* statusCtrl);

	private:

		//获取nirsoft的cmd命令行参数
		std::wstring GetNirsoftCmdstr(PasswordType type);
		std::wstring GetOutputXmlPath(const std::wstring& toolName);
		std::wstring GetNirsoftDir();
		bool unzipNirsoft();

		bool OpenXmlFile(const std::wstring& xmlPath, QDomElement& rootElement); //打开xml文件，获取根节点
		bool ParseBrowser(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseMail(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseBullet(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseMessenger(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseOutlookPst(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseVNC(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseWireless(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseDialupVPN(const std::wstring& xmlPath, CPRecord* parent);

		//wireless输出xml中有括号,不能正常解析，需要处理一下
		std::wstring HandleWirelessXml(const std::wstring& xmlPath);
	};
}

#endif