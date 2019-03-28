#include "stdafx.h"
#include "LogonListInfoHelper.h"

#include <QtXml/QDomDocument>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QString>

namespace app
{
	LogonListInfoHelper::LogonListInfoHelper(void)
	{
	}

	LogonListInfoHelper::~LogonListInfoHelper(void)
	{
	}

	bool LogonListInfoHelper::ParseLogonUserListInformation(vecLogonListInformation &vecLogonUsers)
	{
		bool bRet = psystem::CPOsVersion::isVistaOrLater();
		if (bRet)
		{
			GetVistaLaterLogonUsers(vecLogonUsers);
		}
		else
		{
			GetXPLogonUsers(vecLogonUsers);
		}	
		return true;
	}
	bool LogonListInfoHelper::analyze(CPRecord *parentRecord)
	{
		vecLogonListInformation vecLogonList;
		ParseLogonUserListInformation(vecLogonList);
		if (vecLogonList.empty()) {
			return false;
		}

		for (auto it = vecLogonList.begin(); it != vecLogonList.end(); ++it) {
			CPRecord* record = parentRecord->addChild();
			record->at(L"UserName") = it->at(USERNAME);
			record->at(L"Computer") = it->at(COMPUTER);
			record->at(L"Domain") = it->at(USERDOMIAN);
			record->at(L"LogonTime") = it->at(LOGONTIME);
			record->at(L"LogoffTime") = it->at(LOGOFFTIME);
			record->at(L"Duration") = it->at(DURATION);
		}
		return true;
	}

	bool LogonListInfoHelper::ParseXpUsers(vecLogonListInformation &vecInformaion, const std::wstring& cmd)
	{
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		char seps[] = "\r\n";
		// 单一条记录
		UserInformation single_record;
		std::string strToParse;
		strToParse = std::string(pipe_buf_);
		size_t index = strToParse.find("\r\n");
		size_t oldindex = 0;
		std::deque<std::string> dequeSubStrs;
		boost::find_all(dequeSubStrs, strToParse, "\r\n");
		uint64 iCount = dequeSubStrs.size();
		if (iCount == 0){
			return FALSE;
		}
		int iFinished = 0;

		std::string strLine;

		int offsets[] = {15, 21, 16, 27};
		boost::offset_separator f(offsets, offsets + 4);
		typedef boost::tokenizer<boost::offset_separator> Tokenizer;

		while (index != std::string::npos)
		{
			strLine = strToParse.substr(oldindex, index - oldindex);

			if (strLine.find(std::string("No Records")) != std::string::npos){
				oldindex = index + 2;
				index = strToParse.find("\r\n", oldindex);
				continue;
			}
			if (strLine.substr(0, 16) == std::string(" - End Of File -"))
				break;
			Tokenizer tokens(strLine, f);
			single_record.clear();
			for (Tokenizer::iterator it = tokens.begin(); it != tokens.end(); it++)	{
				if ((*it).size() == 0)
					single_record.push_back(std::wstring(L""));
				else
					single_record.push_back(CPCodeChanger::UTF8ToUnicode(*it));
			}
// 			single_record.push_back(fSucceed ? L"成功" : L"失败");
			single_record.push_back(L"");
			single_record.push_back(L"");
			vecInformaion.push_back(single_record);
			oldindex = index + 2;
			index = strToParse.find("\r\n", oldindex);

		}
		//delete []pipe_buf_;
	//	pipe_buf_ = NULL;
		return TRUE;
	}
	bool LogonListInfoHelper::GetVistaLaterLogonUsers(vecLogonListInformation &vecLogonUsers)
	{
		std::wstring cmd = siutil::CPDirHelper::get()->getToolDir();
		cmd += L"\\userlog\\WinLogOnView.exe";
		cmd = L"\"" + cmd + L"\"";
		cmd += L" /sxml ";
		std::wstring tempPath = siutil::CPDirHelper::get()->getTempDir();
		tempPath += L"\\logonusertemp.xml";
		cmd += L"\"" + tempPath + L"\"";
		std::string pipe_buf_;
		int exitCode = 0;
		if (!CPProcess::createProcess(cmd, L"", &pipe_buf_, &exitCode)){
			return false;
		}
		bool nRet = ParseListXml(tempPath, vecLogonUsers);
		if (!nRet){
			return false;
		}
		DeleteFile(tempPath.c_str());

		return true;
	}
	bool LogonListInfoHelper::GetXPLogonUsers(vecLogonListInformation &vecLogonUsers)
	{
		std::wstring cmd = siutil::CPDirHelper::get()->getToolDir();
		cmd += L"\\userlog\\NTLAST.exe";
		cmd = L"\"" + cmd + L"\"";
		cmd += L" -s";
		if (ParseXpUsers(vecLogonUsers, cmd) == FALSE){
			return false;
		}
		return true;
	}

	bool LogonListInfoHelper::ParseListXml(const std::wstring& wstrXmlPath, vecLogonListInformation &vecLogonUsers)
	{
		QString qstrXmlPath = QString::fromStdWString(wstrXmlPath);
		QFile xmlfile(qstrXmlPath);
		if (!xmlfile.open(QIODevice::ReadOnly)){
			return false; //open failed
		}	
		QDomDocument doc;
		QString errMsg;
		int errLine;
		int errColumn;
		if (!doc.setContent(&xmlfile,false,&errMsg,&errLine,&errColumn)){
			xmlfile.close();
			return false;
		}
		//xml根节点
		QDomElement rootElement = doc.documentElement();
		if (rootElement.tagName() != "user_logon_list")	{
			xmlfile.close();
			return false;
		}
		QDomNodeList childNodes = rootElement.childNodes();
		for (int i=0; i<childNodes.count(); ++i)
		{
			UserInformation single_record;

			QDomNode childNode = childNodes.at(i);
			single_record.push_back(childNode.firstChildElement(QString("user_name")).text().toStdWString());
			single_record.push_back(childNode.firstChildElement(QString("computer")).text().toStdWString());
			single_record.push_back(childNode.firstChildElement(QString("domain")).text().toStdWString());
			single_record.push_back(childNode.firstChildElement(QString("logon_time")).text().toStdWString());
			single_record.push_back(childNode.firstChildElement(QString("logoff_time")).text().toStdWString());
			single_record.push_back(childNode.firstChildElement(QString("duration")).text().toStdWString());
			vecLogonUsers.push_back(single_record);
		}
		return true;
	}

}

