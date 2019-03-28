#include "stdafx.h"
#include <clocale>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QFile>
#include <boost/regex.hpp>
#include <boost/regex/pattern_except.hpp>
#include "MacIMAnalyzer/include/macim/adium/PAdiumAnalyzer.h"

namespace macbasic {
    CPAdiumAnalyzer::CPAdiumAnalyzer(void)
    {
    }


    CPAdiumAnalyzer::~CPAdiumAnalyzer(void)
    {
        try
        {
            Clear();
        }
        catch(...)
        {
        }
    }


    /**
    * @brief	第一个子节点
    * @param	[in]:	void
    * @return	const CPAccount *
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    CPAccount * CPAdiumAnalyzer::FirstChildAccount()
    {
        if (m_dequeAccount.empty())
        {
            return NULL;
        }

        m_AccountCurrent = m_dequeAccount.begin();
        return *m_AccountCurrent;
    }

    /**
    * @brief	下一个子节点
    * @param	[in]:	void
    * @return	const CPAccount *
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    CPAccount * CPAdiumAnalyzer::NextChildAccount()
    {
        if (m_dequeAccount.empty()) {
            return NULL;
        }

        ++m_AccountCurrent;
        if (m_dequeAccount.end() == m_AccountCurrent) {
            return NULL;
        }

        return *m_AccountCurrent;
    }

    /**
    * @brief	添加一个accounts文件
    * @param	[in]:	std::wstring
    * @return	bool
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    bool CPAdiumAnalyzer::AddAccounts(std::wstring wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }
        m_dequeAccounts.push_back(wstrFile);
        if (wstrFile != m_dequeAccounts.back()) {
            return false;
        }
        return AccountsAnalyzer(wstrFile);
    }

    bool CPAdiumAnalyzer::AccountsAnalyzer(const std::wstring & wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }

        QString qStrPath = QString::fromStdWString(wstrFile);
        try
        {
            QFile file(qStrPath);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                return false;
            }

            QString errorStr;
            int errorLine;
            int errorColumn;
            QDomDocument doc;
            if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) { //填充 DOM
                file.close();
                return false;
            }
            QDomElement root = doc.documentElement();
            CPAccount * pAccount = NULL;
            const char *pOld = setlocale(LC_ALL, "");
            for(QDomElement qAccount= root.firstChildElement("account");
                !qAccount.isNull(); qAccount= qAccount.nextSiblingElement("account")) {
                pAccount = new CPAccount;

                // protocol
                QDomElement qElement = qAccount.firstChildElement("protocol");
                QString strValue = qElement.text();
                if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-msn")) { // MSN
                    pAccount->m_adiumProtocol = ADIUM_MSN;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-qq")) { // QQ
                    pAccount->m_adiumProtocol = ADIUM_QQ;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-aol")) { // AOL
                    pAccount->m_adiumProtocol = ADIUM_AOL;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-icq")) { // ICQ
                    pAccount->m_adiumProtocol = ADIUM_ICQ;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-gtalk")) { // GTALK
                    pAccount->m_adiumProtocol = ADIUM_GTALK;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-yahoo")) { // YAHOO
                    pAccount->m_adiumProtocol = ADIUM_YAHOO;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-fb")) { // FACEBOOK
                    pAccount->m_adiumProtocol = ADIUM_FB;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-msi")) { // MSI
                    pAccount->m_adiumProtocol = ADIUM_MSI;
                }
                else if (0 == strcasecmp(strValue.toStdString().c_str(), "prpl-lotus")) { // LOTUS
                    pAccount->m_adiumProtocol = ADIUM_LOTUS;
                }

                // name
                qElement = qAccount.firstChildElement("name");
                strValue = qElement.text();
                pAccount->m_wstrAccount = strValue.toStdWString();
                strValue.clear();

                // else

                m_dequeAccount.push_back(pAccount);
                pAccount = NULL;
            }
            setlocale(LC_ALL, pOld);
            file.close();
        }
        catch (...) {
            return false;
        }
        return true;
    }

    CPContact *CPAdiumAnalyzer::FirstChildContact()
    {
        if (m_dequeContact.empty()) {
            return NULL;
        }
        m_ContactCurrent = m_dequeContact.begin();
        return *m_ContactCurrent;
    }

    CPContact *CPAdiumAnalyzer::NextChildContact()
    {
        if (m_dequeContact.empty()) {
            return NULL;
        }
        ++m_ContactCurrent;
        if (m_dequeContact.end() == m_ContactCurrent) {
            return NULL;
        }
        return *m_ContactCurrent;
    }

    /**
    * @brief	添加一个blist 文件
    * @param	[in]:	std::wstring
    * @return	bool
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    bool CPAdiumAnalyzer::AddBlist(std::wstring wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }
        m_dequeBlist.push_back(wstrFile);
        if (wstrFile != m_dequeBlist.back()) {
            return false;
        }
        return BlistAnalyzer(wstrFile);
    }

    /**
    * @brief	Blist 文件分析器
    * @param	[in]:	std::wstring
    * @return	bool
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    bool CPAdiumAnalyzer::BlistAnalyzer(const std::wstring & wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }

        QString qStrPath = QString::fromStdWString(wstrFile);
        try
        {
            QFile file(qStrPath);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                return false;
            }

            QString errorStr;
            int errorLine;
            int errorColumn;
            QDomDocument doc;
            if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) { //填充 DOM
                file.close();
                return false;
            }
            QDomElement root = doc.documentElement();

            QDomElement qBlistElement = root.firstChildElement("blist");
            if (qBlistElement.isNull()) {
                file.close();
                return false;
            }

            CPContact *pContact = NULL;
            const char *pOld = setlocale(LC_ALL, "");

            // 查找所属组
            for (QDomElement qGroup = qBlistElement.firstChildElement("group");
                 !qGroup.isNull(); qGroup = qGroup.nextSiblingElement("group")) {
                QString strGroup = qGroup.attribute(QString("name"));
                if (strGroup.isNull()) {
                    continue;
                }
                // 查找其他信息
                for (QDomElement qContact = qGroup.firstChildElement("contact");
                     !qContact.isNull(); qContact = qContact.nextSiblingElement("contact")) {
                    pContact = new CPContact;
                    if (NULL == pContact) {
                        continue;
                    }
                    // 所属组
                    pContact->m_wstrGroupName = strGroup.toStdWString();
                    QDomElement qBuddy = qContact.firstChildElement("buddy");
                    // 所属账号
                    QString strValue = qBuddy.attribute("account");
                    pContact->m_wstrBelongs = strValue.toStdWString();
                    strValue.clear();
                    // 账号
                    QDomElement qElement = qBuddy.firstChildElement("name");
                    strValue = qElement.text();
                    pContact->m_wstrAccount = strValue.toStdWString();
                    strValue.clear();
                    // 别名
                    qElement = qBuddy.firstChildElement("alias");
                    strValue = qElement.text();
                    pContact->m_wstrAlias = strValue.toStdWString();
                    strValue.clear();
                    // else

                    m_dequeContact.push_back(pContact);
                    pContact = NULL;
                }
            }
            setlocale(LC_ALL, pOld);
            file.close();
        }
        catch (...) {
            return false;
        }
        return true;
    }

    CPMsgRecord *CPAdiumAnalyzer::FirstChildMsg()
    {
        if (m_dequeMsg.empty()) {
            return NULL;
        }
        m_MsgCurrent = m_dequeMsg.begin();
        return *m_MsgCurrent;
    }

    CPMsgRecord *CPAdiumAnalyzer::NextChildMsg()
    {
        if (m_dequeMsg.empty()) {
            return NULL;
        }
        ++m_MsgCurrent;
        if (m_dequeMsg.end() == m_MsgCurrent) {
            return NULL;
        }
        return *m_MsgCurrent;
    }

    /**
     * @name    AddMessage
     * @brief   添加一个聊天记录文件
     * @param   [in]:   std::wstring
     * @return  bool
     * @author  Jesse.Xu (xgs), xgs@pansafe.com
     */
    bool CPAdiumAnalyzer::AddMessage(std::wstring wstrFile)
    {
        if (wstrFile.substr(wstrFile.length() - 4) == L".xml")
        {
            return AddChatlog(wstrFile);
        }

        if (wstrFile.substr(wstrFile.length() - 5) == L".html")
        {
            return AddHtmllog(wstrFile);
        }
    }

    /**
    * @brief	添加一个chatlog 文件
    * @param	[in]:	std::wstring
    * @return	bool
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    bool CPAdiumAnalyzer::AddChatlog(std::wstring wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }

        m_dequeChatlog.push_back(wstrFile);
        return ChatlogAnalyzer(wstrFile);
    }

    /**
     * @name    AddHtmllog
     * @brief   添加一个新版本的聊天记录文件
     * @param   [in]:   const std::wstring &
     * @return  bool
     * @author  Jesse.Xu (xgs), xgs@pansafe.com
     * @date    2009.11.16
     */
    bool CPAdiumAnalyzer::AddHtmllog(const std::wstring & wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }
        m_dequeChatlog.push_back(wstrFile);
        return HtmllogAnalyzer(wstrFile);
    }

    /**
    * @brief	chatlog 文件分析器
    * @param	[in]:	std::wstring
    * @return	bool
    * @author	nct(xgs@pansafe.com)
    * @date	2009.06.01
    */
    bool CPAdiumAnalyzer::ChatlogAnalyzer(const std::wstring & wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }

        QString qStrPath = QString::fromStdWString(wstrFile);
        try
        {
            QFile file(qStrPath);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                return false;
            }

            QString errorStr;
            int errorLine;
            int errorColumn;
            QDomDocument doc;
            if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn)) { //填充 DOM
                file.close();
                return false;
            }
            QDomElement root = doc.documentElement();
            if (root.isNull()) {
                return false;
            }
            QString strBelongs = root.attribute("account");
            if (strBelongs.isNull()) {
                return false;
            }

    #ifndef __linux__
            std::wstring wstrFileName = wstrFile.substr(wstrFile.rfind(L'\\') + 1);
    #else
            std::wstring wstrFileName = wstrFile.substr(wstrFile.rfind(L'/') + 1);
    #endif // __linux__

            std::wstring wstrContact = wstrFileName.substr(0, wstrFileName.find(L'(') - 1); // 联系人

            // 聊天信息提取
            CPMsgRecord * newMsg = NULL;
            const char * pOld = setlocale(LC_ALL, "");
            for (QDomElement qMessage = root.firstChildElement("message");
                 !qMessage.isNull(); qMessage = qMessage.nextSiblingElement("message")) {
                newMsg = new CPMsgRecord;
                // 消息发送者
                QString strValue = qMessage.attribute("sender");
                newMsg->m_wstrSender = strValue.toStdWString();
                strValue.clear();
                // 消息接收者
                if (newMsg->m_wstrSender == strBelongs.toStdWString()) {
                    newMsg->m_wstrReceiver = wstrContact;
                }
                else if (newMsg->m_wstrSender == wstrContact){
                    newMsg->m_wstrReceiver = strBelongs.toStdWString();
                }
                // 消息的时间
                strValue = qMessage.attribute("time");
                newMsg->m_wstrTime = strValue.toStdWString().substr(0, 10) + L" "
                        + strValue.toStdWString().substr(11, 8);
                strValue.clear();
                // 消息内容
                QString strHref = "";
                QDomElement qDiv = qMessage.firstChildElement("div");
                if (!qDiv.isNull()) {
                    strValue = qDiv.text();
                    QDomElement qA = qDiv.firstChildElement();
                    if ("a" == qA.nodeName()) {
                        //
                    }
                    else if ("span" == qA.nodeName()) {
                        qA = qA.firstChildElement("a");
                    }

                    if (!qA.isNull()) {
                        strHref = qA.attribute("href");
                    }
                    if (!strHref.isEmpty()) {
                        strValue = "[链接]: " + strValue + " " + strHref;
                    }
                }
                newMsg->m_wstrMsgText = strValue.toStdWString();
                strValue.clear();
                // else

                m_dequeMsg.push_back(newMsg);
                newMsg = NULL;
            }
            setlocale(LC_ALL, pOld);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    /**
     * @name    HtmllogAnalyzer
     * @brief   新版本adium的聊天记录分析
     * @param   [in]:   const std::wstring &
     * @return  bool
     * @author  Jesse.Xu (xgs), xgs@pansafe.com
     */
    bool CPAdiumAnalyzer::HtmllogAnalyzer(const std::wstring & wstrFile)
    {
        if (wstrFile.empty())
        {
            return false;
        }

        const int SIZE = 266;
        char szPath[SIZE] = {0};
        const char * pOld = setlocale(LC_ALL, "");
        wcstombs(szPath, wstrFile.c_str(), SIZE);
        setlocale(LC_ALL, pOld);


        // 暂时使用字符串匹配代码html解析, 不能作xml解析
        std::string strTimeStamp("<span class=\"timestamp\">");
        std::string strSender("<span class=\"sender\">");
        std::string strMessage("<pre class=\"message\">");
        std::string strLine;
        const int DATA_SIZE = 4096;
        wchar_t wcsData[DATA_SIZE] = {0};
        std::ifstream ifs(szPath);
        if (ifs.is_open())
        {
            while(getline(ifs, strLine))
            {
                size_t stPosBegin = 0;
                size_t stPosEnd = 0;
                // 消息发送时间
                stPosBegin = strLine.find(strTimeStamp, stPosBegin);
                if (std::string::npos == stPosBegin)
                {
                    continue;
                }
                stPosEnd = strLine.find("</span>", stPosBegin);
                if (std::string::npos == stPosEnd)
                {
                    continue;
                }

                size_t stPosTime = stPosBegin + strTimeStamp.length();
                if (stPosTime >= stPosEnd)
                {
                    continue;
                }
                std::string strTime(strLine.substr(stPosTime, stPosEnd - stPosTime));

                // 消息发送者
                stPosBegin = strLine.find(strSender, stPosEnd);
                if (std::string::npos == stPosBegin)
                {
                    continue;
                }
                stPosEnd = strLine.find("</span>", stPosBegin);
                if (std::string::npos == stPosEnd)
                {
                    continue;
                }
                size_t stPosSender = stPosBegin + strSender.length();
                if (stPosSender >= stPosEnd)
                {
                    continue;
                }
                std::string strSender(strLine.substr(stPosSender, stPosEnd - stPosSender));
                strSender = strSender.substr(0, strSender.rfind(":"));

                // 消息
                std::string strMsg;
                size_t stPosMessage = 0;
                stPosBegin = strLine.find(strMessage, stPosEnd);
                if (std::string::npos != stPosBegin)
                {
                    stPosEnd = strLine.find("</pre>", stPosBegin);
                    if (std::string::npos !=stPosEnd)
                    {
                        stPosMessage = stPosBegin + strMessage.length();
                    }
                }
                if (stPosMessage <= stPosEnd)
                {
                    strMsg = strLine.substr(stPosMessage, stPosEnd - stPosMessage);
                }

                // 添加聊天记录
                CPMsgRecord newMsg;
                memset(wcsData, 0, sizeof(wchar_t) * DATA_SIZE);
                mbstowcs(wcsData, strSender.c_str(), DATA_SIZE * sizeof(wchar_t));
                newMsg.m_wstrSender = wcsData;
                memset(wcsData, 0, sizeof(wchar_t) * DATA_SIZE);
                mbstowcs(wcsData, strMsg.c_str(), DATA_SIZE * sizeof(wchar_t));
                newMsg.m_wstrMsgText = wcsData;
                memset(wcsData, 0, sizeof(wchar_t) * DATA_SIZE);
                mbstowcs(wcsData, strTime.c_str(), DATA_SIZE * sizeof(wchar_t));
                newMsg.m_wstrTime = wcsData;
                m_dequeMsg.push_back(&newMsg);

            } // End of WHILE getline

        } // End of IF is_open

        return true;
    }

    /*
    * 清理函数
    */
    void CPAdiumAnalyzer::Clear()
    {
        if (!m_dequeAccount.empty())
        {
            for (std::deque<CPAccount *>::iterator it = m_dequeAccount.begin();
                it != m_dequeAccount.end(); ++it)
            {
                if (NULL == *it)
                {
                    continue;
                }

                delete *it;
                *it = NULL;
            }

            m_dequeAccount.clear();
        }



        if (!m_dequeContact.empty()) {
            for (std::deque<CPContact *>::iterator it = m_dequeContact.begin();
                 it != m_dequeContact.end(); ++it) {
                if (NULL == *it) {
                    continue;
                }
                delete *it;
                *it = NULL;
            }
            m_dequeContact.clear();
        }


        if (!m_dequeMsg.empty()) {
            for (std::deque<CPMsgRecord *>::iterator it = m_dequeMsg.begin();
                 it != m_dequeMsg.end(); ++it) {
                if (NULL == *it) {
                    continue;
                }
                delete *it;
                *it = NULL;
            }
            m_dequeMsg.clear();
        }

    }

    // 修剪
    std::wstring CPAdiumAnalyzer::trim(const std::wstring & wstrIn)
    {
        std::wstring wstrSpace(L" \t\n\r");
        size_t stPos1 = wstrIn.find_first_not_of(wstrSpace);
        if (std::wstring::npos == stPos1)
        {
            return wstrIn;
        }

        size_t stPos2 = wstrIn.find_last_not_of(wstrSpace, stPos1);
        if (std::wstring::npos == stPos2 || stPos2 <= stPos1)
        {
            return wstrIn.substr(stPos1);
        }

        return wstrIn.substr(stPos1, stPos2 - stPos1);
    }


    time_t CPAdiumAnalyzer::ProcessMsgTime(const std::wstring & wstrFile, const std::string & strTime)
    {
        size_t stEnd = wstrFile.rfind(L").html");
        if (std::wstring::npos == stEnd)
        {
            return 0;
        }

        size_t stBegin = wstrFile.rfind(L"(", stEnd);
        if (std::wstring::npos == stBegin)
        {
            return 0;
        }

        if (stBegin >= stEnd)
        {
            return 0;
        }

        std::wstring wstrDate(wstrFile.substr(stBegin + 1, stEnd - stBegin - 1));
        const int SIZE = 128;
        char szDate[SIZE] = {0};
        wcstombs(szDate, wstrDate.c_str(), SIZE);
        std::string strFullTime(szDate);
        strFullTime += strTime;

        struct tm tmMsg;
        strptime(strFullTime.c_str(), "%Y-%m-%e%p%I:%M:%S", &tmMsg);
        time_t ttRet = mktime(&tmMsg);
        return ttRet > 0 ? ttRet : 0;

        return 0;
    }
}


