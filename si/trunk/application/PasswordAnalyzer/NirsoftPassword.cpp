#include "stdafx.h"
#include "NirSoftPassword.h"

#include "application/PTaskError.h"

#include <QtCore/QProcess>
#include <QtCore/QFile>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>

#include "siutility/PFileHandler.h"

using boost::property_tree::wptree;


namespace password
{
	//由于需要判断并解压密码工具，为防止多线程冲突，这里加一个锁
	std::mutex g_pwMutex;

	CNirsoftPassword::CNirsoftPassword()
	{

	}

	CNirsoftPassword::~CNirsoftPassword()
	{

	}

	std::wstring CNirsoftPassword::GetNirsoftDir()
	{
		std::wstring toolsDir = siutil::CPDirHelper::get()->getToolDir();
		std::wstring nirDir = toolsDir + L"\\password\\nirsoft";
		return nirDir;
	}

	bool CNirsoftPassword::unzipNirsoft()
	{
		//如果密码工具丢失，从压缩包里重新解压一下，密码：123
		std::wstring nirDir = GetNirsoftDir();
		std::wstring toolDir = siutil::CPDirHelper::get()->getToolDir();
		std::wstring nirZipPath = toolDir + L"\\password\\nirsoft.7z";
		return siutil::CPFileHandler::unzip(nirZipPath, nirDir);
	}

	std::wstring CNirsoftPassword::GetNirsoftCmdstr( PasswordType type )
	{
		std::lock_guard<std::mutex> lock(g_pwMutex);

		std::wstring toolName = CPswdTypeTablename::Instance()->GetToolName(type);

		std::wstring nirsoftDir = GetNirsoftDir();
		std::wstring toolFullName = toolName + L".exe";
		std::wstring toolPath = nirsoftDir + L"\\" + toolFullName;

		try {
			if (!boost::filesystem::exists(toolPath)) {
				if (!unzipNirsoft()) {
					return L"";
				}
			}
		}
		catch (...) {

		}

		std::wstring xmlPath = GetOutputXmlPath(toolName);
		std::wstring toolPathstr = L"\"" + toolPath + L"\""; //在命令行中两边要加上引号，下同
		std::wstring xmlPathstr = L"\"" +xmlPath + L"\"";
		std::wstring cmdstr = toolPathstr + L" /sxml " + xmlPathstr;

		return cmdstr;
	}

	std::wstring CNirsoftPassword::GetOutputXmlPath(const std::wstring& toolName )
	{
		std::wstring tempDir = siutil::CPDirHelper::get()->getTempDir();
		std::wstring outPath = tempDir + L"\\nirsoft\\";
		try
		{
			if (!boost::filesystem::exists(outPath))
			{
				boost::filesystem::create_directories(outPath);
			}
		}
		catch(...)
		{
			return L"";
		}
		std::wstring xmlFullName = toolName + L".xml";
		std::wstring xmlPath = outPath + L"/"  + xmlFullName;

		return xmlPath;
	}

	bool CNirsoftPassword::CrackNirsoftPassword(const std::wstring& typeName,
		CPRecord* parentRecord, CPTaskStatusCtrl* statusCtrl)
	{
		PasswordType type = CPswdTypeTablename::Instance()->GetPsType(typeName);
		std::wstring toolName = CPswdTypeTablename::Instance()->GetToolName(type);
		std::wstring cmd = GetNirsoftCmdstr(type);
		if (cmd.empty()) {
			statusCtrl->fail(TASK_ERROR_TOOL_LOST);
			return false;
		}
		int exitCode = QProcess::execute(QString::fromStdWString(cmd));
		if (exitCode != 0) {
			statusCtrl->fail(TASK_ERROR_TOOL_LOST);
			return false;
		}

		std::wstring xmlPath = GetOutputXmlPath(toolName);

		if (type == Pswd_Browser)
		{
			ParseBrowser(xmlPath, parentRecord);
		}
		else if (type == Pswd_Mail)
		{
			ParseMail(xmlPath, parentRecord);
		}
		else if (type == Pswd_Bullet)
		{
			ParseBullet(xmlPath, parentRecord);
		}
		else if (type == Pswd_Messenger)
		{
			ParseMessenger(xmlPath, parentRecord);
		}
		else if (type == Pswd_OutlookPst)
		{
			ParseOutlookPst(xmlPath, parentRecord);
		}
		else if (type == Pswd_VNC)
		{
			ParseVNC(xmlPath, parentRecord);
		}
		else if (type == Pswd_Wireless)
		{
			xmlPath = HandleWirelessXml(xmlPath);
			ParseWireless(xmlPath, parentRecord);
		}
		else if (type == Pswd_DialupVPN)
		{
			ParseDialupVPN(xmlPath, parentRecord);
		}
		else {
			return false;
		}

		try
		{
			boost::filesystem::remove(xmlPath); //删除输出的xml文件
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	bool CNirsoftPassword::OpenXmlFile(const std::wstring& xmlPath, QDomElement& rootElement )
	{
		QDomDocument doc;
		QFile xmlfile(QString::fromStdWString(xmlPath));
		if (!xmlfile.open(QIODevice::ReadOnly))
		{
			return false; //open failed
		}

		QString errMsg;
		int errLine;
		int errColumn;

		if (!doc.setContent(&xmlfile,false,&errMsg,&errLine,&errColumn))
		{
			xmlfile.close();
			return false;
		}
		xmlfile.close();

		rootElement = doc.documentElement();
		return true;
	}

	bool CNirsoftPassword::ParseBrowser(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "web_browser_passwords") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i) {
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item") {
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j) {
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "web_browser") {
					record->at(L"BrowserType") = child.text().toStdWString();
				}
				else if (child.tagName() == "url") {
					record->at(L"url") = child.text().toStdWString();
				}
				else if (child.tagName() == "user_name") {
					record->at(L"UserName") = child.text().toStdWString();
				}
				else if (child.tagName() == "password") {
					record->at(L"Password") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseMail(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "email_accounts_list") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i) {
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item") {
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j) {
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "application") {
					record->at(L"MailType") = child.text().toStdWString();
				}
				else if (child.tagName() == "email") {
					record->at(L"EmailAddress") = child.text().toStdWString();
				}
				else if (child.tagName() == "password") {
					record->at(L"Password") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseBullet(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "passwords_list") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i) {
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item") {
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j) {
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "window_title") {
					record->at(L"WindowTitle") = child.text().toStdWString();
				}
				else if (child.tagName() == "password") {
					record->at(L"Password") = child.text().toStdWString();
				}
				else if (child.tagName() == "process_name") {
					record->at(L"ProcessName") = child.text().toStdWString();
				}
				else if (child.tagName() == "process_path") {
					record->at(L"ProcessPath") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseMessenger(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "instant_messengers_accounts_list") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i) {
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item") {
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j) {
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "software") {
					record->at(L"SoftwareType") = child.text().toStdWString();
				}
				else if (child.tagName() == "user") {
					record->at(L"User") = child.text().toStdWString();
				}
				else if (child.tagName() == "password") {
					record->at(L"Password") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseOutlookPst(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "pst_passwords_list") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i) {
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item") {
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j) {
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "crc_value") {
					record->at(L"CRCValue") = child.text().toStdWString();
				}
				else if (child.tagName() == "password_1") {
					record->at(L"Password1") = child.text().toStdWString();
				}
				else if (child.tagName() == "password_2") {
					record->at(L"Password2") = child.text().toStdWString();
				}
				else if (child.tagName() == "password_3") {
					record->at(L"Password3") = child.text().toStdWString();
				}
				else if (child.tagName() == "full_path") {
					record->at(L"FilePath") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseVNC(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem)) {
			return false;
		}

		if (rootElem.tagName() != "vnc_passwords") {
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i)
		{
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item")
			{
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j)
			{
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "password_type") {
					record->at(L"PasswordType") = child.text().toStdWString();
				}
				else if (child.tagName() == "version") {
					record->at(L"Version") = child.text().toStdWString();
				}
				else if (child.tagName() == "password") {
					record->at(L"Password") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseWireless(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem))
		{
			return false;
		}

		if (rootElem.tagName() != "wireless_keys_list")
		{
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i)
		{
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item")
			{
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j)
			{
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "network_name_ssid") {
					record->at(L"NetworkName") = child.text().toStdWString();
				}
				else if (child.tagName() == "key_type") {
					record->at(L"KeyType") = child.text().toStdWString();
				}
				else if (child.tagName() == "key_ascii") {
					record->at(L"PasswordAscii") = child.text().toStdWString();
				}
				else if (child.tagName() == "adapter_name") {
					record->at(L"AdapterName") = child.text().toStdWString();
				}
				else if (child.tagName() == "encryption") {
					record->at(L"Encryption") = child.text().toStdWString();
				}
				else if (child.tagName() == "connection_type") {
					record->at(L"ConnectionType") = child.text().toStdWString();
				}
				else if (child.tagName() == "last_modified") {
					record->at(L"LastModified") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	bool CNirsoftPassword::ParseDialupVPN(const std::wstring& xmlPath, CPRecord* parent)
	{
		QDomElement rootElem;
		if (!OpenXmlFile(xmlPath, rootElem))
		{
			return false;
		}

		if (rootElem.tagName() != "dialup_vpn_passwords_list")
		{
			return false;
		}

		QDomNodeList rootList = rootElem.childNodes();
		int listSize = rootList.size();
		for(int i = 0; i < listSize; ++i)
		{
			QDomElement itemElem = rootList.at(i).toElement();
			if (!itemElem.isElement() || itemElem.tagName() != "item")
			{
				return false;
			}

			CPRecord* record = parent->addChild();

			QDomNodeList itemList = itemElem.childNodes();
			for (int j = 0; j < itemList.size(); ++j)
			{
				QDomElement& child = itemList.at(j).toElement();
				if (child.tagName() == "entry_name") {
					record->at(L"EntryName") = child.text().toStdWString();
				}
				else if (child.tagName() == "phone___host")
				{
					record->at(L"PhoneHost") = child.text().toStdWString();
				}
				else if (child.tagName() == "user_name")
				{
					record->at(L"UserName") = child.text().toStdWString();
				}
				else if (child.tagName() == "password")
				{
					record->at(L"Password") = child.text().toStdWString();
				}
				else if (child.tagName() == "domain")
				{
					record->at(L"Domain") = child.text().toStdWString();
				}
				else if (child.tagName() == "password_owner")
				{
					record->at(L"PasswordOwner") = child.text().toStdWString();
				}
				else if (child.tagName() == "password_owner_sid")
				{
					record->at(L"PasswordOwnerSid") = child.text().toStdWString();
				}
			}
		}
		return true;
	}

	std::wstring CNirsoftPassword::HandleWirelessXml(const std::wstring& xmlPath)
	{
		std::ifstream in(xmlPath);
		std::wstring outPath = xmlPath + L"2";
		try
		{
			std::ofstream out(outPath);
			std::string line;
			while (getline(in,line))
			{
				boost::replace_all(line, "<network_name_(ssid)>", "<network_name_ssid>");
				boost::replace_all(line, "</network_name_(ssid)>", "</network_name_ssid>");
				boost::replace_all(line, "<key_(hex)>", "<key_hex>");
				boost::replace_all(line, "</key_(hex)>", "</key_hex>");
				boost::replace_all(line, "<key_(ascii)>", "<key_ascii>");
				boost::replace_all(line, "</key_(ascii)>", "</key_ascii>");
				line.append("\n");

				out << line;
			}
			in.close();
			out.close();

			boost::filesystem::remove(xmlPath); //删除原来的文件
		}
		catch(...)
		{}

		return outPath;
	}
}