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

		//��ȡnirsoft��cmd�����в���
		std::wstring GetNirsoftCmdstr(PasswordType type);
		std::wstring GetOutputXmlPath(const std::wstring& toolName);
		std::wstring GetNirsoftDir();
		bool unzipNirsoft();

		bool OpenXmlFile(const std::wstring& xmlPath, QDomElement& rootElement); //��xml�ļ�����ȡ���ڵ�
		bool ParseBrowser(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseMail(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseBullet(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseMessenger(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseOutlookPst(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseVNC(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseWireless(const std::wstring& xmlPath, CPRecord* parent);
		bool ParseDialupVPN(const std::wstring& xmlPath, CPRecord* parent);

		//wireless���xml��������,����������������Ҫ����һ��
		std::wstring HandleWirelessXml(const std::wstring& xmlPath);
	};
}

#endif