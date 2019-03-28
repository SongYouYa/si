#include "stdafx.h"
#include "PImessageAnalyzer.h"
#include <stdlib.h>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
namespace macbasic {
    CPImessageAnalyzer::CPImessageAnalyzer()
    {

    }

    CPImessageAnalyzer::~CPImessageAnalyzer()
    {
        try
        {
            Clear();
        }
        catch(...)
        {
        }
    }

    CPIMAccount * CPImessageAnalyzer::FirstChildAccount()
    {
        if (m_dequeAccount.empty()) {
            return NULL;
        }
        m_itCurrent = m_dequeAccount.begin();
        return *m_itCurrent;
    }

    CPIMAccount * CPImessageAnalyzer::NextChildAccount()
    {
        if (m_dequeAccount.empty()) {
            return NULL;
        }

        ++m_itCurrent;
        if (m_dequeAccount.end() == m_itCurrent) {
            return NULL;
        }

        return *m_itCurrent;
    }

    bool CPImessageAnalyzer::addPlist(const std::wstring &wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }
        m_dequePlist.push_back(wstrFile);
        if (wstrFile != m_dequePlist.back()) {
            return false;
        }
        return plistAnalyzer(wstrFile);
    }

    bool CPImessageAnalyzer::plistAnalyzer(const std::wstring &wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }

        std::string szPath =CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());
        try {
            plistcpp::dictionary_type dict;
            plistcpp::CPlistCpp::ReadPlist(szPath.c_str(), dict);
            plistcpp::dictionary_type::iterator it = dict.find("Accounts");
            if (dict.end() == it) {
                return false;
            }
            dict = boost::any_cast<plistcpp::dictionary_type>(it->second);
            CPIMAccount * pCurrentAccount = NULL;
            std::wstring wstr = L"";
            for (it = dict.begin(); it != dict.end(); ++it) {
                plistcpp::dictionary_type d = boost::any_cast<plistcpp::dictionary_type>(it->second);
                plistcpp::dictionary_type::iterator strIt = d.find("LoginAs");
                if (d.end() == strIt) {
                     continue;
                }
                std::string strText = boost::any_cast<std::string>(strIt->second);
                wstr = CPCodeChanger::UTF8ToUnicode(strText.c_str());
                pCurrentAccount = new CPIMAccount;
                if (NULL == pCurrentAccount) {
                    continue;
                }
                // account
                pCurrentAccount->m_wstrAccount = wstr;
                strText.clear();
                wstr.clear();

                // type
                size_t posTypeBegin = wstrFile.rfind(L"com.macbasicle.iChat.");
                if (std::wstring::npos == posTypeBegin) {
                    continue;
                }
                size_t posTypeEnd = wstrFile.rfind(L".");
                if (std::wstring::npos == posTypeEnd) {
                    continue;
                }
                std::wstring wstrType = wstrFile.substr(posTypeBegin + 16, posTypeEnd -posTypeBegin -16);
                if (std::wstring::npos != (wstrType.find(L"AIM"))) {
                    pCurrentAccount->m_wstrType = L"AIM";
                }
                else if (std::wstring::npos != (wstrType.find(L"Jabber"))) {
                    pCurrentAccount->m_wstrType = L"Jabber";
                }
                else if (std::wstring::npos != (wstrType.find(L"Yahoo"))) {
                    pCurrentAccount->m_wstrType = L"Yahoo";
                }
                else {
                    pCurrentAccount->m_wstrType = wstrType;
                }

                // belongs system
                size_t posBegin = wstrFile.find(L"/Users/");
                if (std::wstring::npos == posBegin) {
                    continue;
                }
                size_t posEnd = wstrFile.find(L"/", posBegin + 7);
                if (std::wstring::npos == posEnd) {
                    continue;
                }
                std::wstring wstrBelongs = wstrFile.substr(posBegin + 7, posEnd - posBegin - 7);
                pCurrentAccount->m_wstrBelongsSystem = wstrBelongs;

                m_dequeAccount.push_back(pCurrentAccount);
            }
        }
        catch (...) {
            return false;
        }
        return true;
    }

    CPIMMessage *CPImessageAnalyzer::FirstChildMsg()
    {
        if (m_dequeMessage.empty()) {
            return NULL;
        }
        m_itCurrentM = m_dequeMessage.begin();
        return *m_itCurrentM;
    }

    CPIMMessage *CPImessageAnalyzer::NextChildMsg()
    {
        if (m_dequeMessage.empty()) {
            return NULL;
        }
        ++m_itCurrentM;
        if (m_dequeMessage.end() == m_itCurrentM) {
            return NULL;
        }
        return *m_itCurrentM;
    }

    bool CPImessageAnalyzer::addMessage(const std::wstring &wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }
        m_dequeChatDb.push_back(wstrFile);
        if (wstrFile != m_dequeChatDb.back()) {
            return false;
        }
        return messageAnalyzer(wstrFile);
    }

    bool CPImessageAnalyzer::messageAnalyzer(const std::wstring &wstrFile)
    {
        if (wstrFile.empty()) {
            return false;
        }
        sqlite3 * db = NULL;

        std::string szPath = CPCodeChanger::UnicodeToUTF8(wstrFile.c_str());

        try {
            if (sqlite3_open(szPath.c_str(), &db)) {
                sqlite3_close(db);
                return false;
            }
            if (!db) {
                return false;
            }

            int fileTransferCount = 0; //第几个文件传输消息
            sqlite3_stmt * pStmt = NULL;
            if (SQLITE_OK != sqlite3_prepare(db, "select account,text,date,handle_id,is_from_me, cache_has_attachments from message",
                                             -1,  &pStmt, NULL)) {
                return false;
            }

            CPIMMessage *pCurrentMsg = NULL;
            while (SQLITE_ROW == sqlite3_step(pStmt)) {
                pCurrentMsg = new CPIMMessage;
                if (NULL == pCurrentMsg) {
                    continue;
                }
                std::string strAccount = (const char*)sqlite3_column_text(pStmt, 0);

                std::string strText = (const char*)sqlite3_column_text(pStmt, 1);
                pCurrentMsg->m_wstrMsg = CPCodeChanger::UTF8ToUnicode(strText.c_str());

                double dTime = sqlite3_column_double(pStmt, 2);
                time_t ttTime = MAC_BASE_TIME + dTime;
                pCurrentMsg->m_CPTime = CPTime::UTC2LocalTime(ttTime, siutil::MacCommonHelper::GetTimezoneRegion());

                std::string handleId = (const char*)sqlite3_column_text(pStmt, 3);
                sqlite3_stmt * pStmtHandle = NULL;
                std::string sqlWord = "select id from handle where ROWID = " + handleId;
                if (SQLITE_OK != sqlite3_prepare(db, sqlWord.c_str(), -1,  &pStmtHandle, NULL)) {
                    delete pCurrentMsg;
                    continue;
                }
                if (SQLITE_ROW != sqlite3_step(pStmtHandle)) {
                    delete pCurrentMsg;
                    continue;
                }
                std::string strHandle = (const char*)sqlite3_column_text(pStmtHandle, 0);

                if (sqlite3_column_int(pStmt, 4)) {
                    pCurrentMsg->m_wstrSender = CPCodeChanger::UTF8ToUnicode(strAccount.c_str());
                    pCurrentMsg->m_wstrReceiver = CPCodeChanger::UTF8ToUnicode(strHandle.c_str());
                }
                else {
                    pCurrentMsg->m_wstrSender = CPCodeChanger::UTF8ToUnicode(strHandle.c_str());
                    pCurrentMsg->m_wstrReceiver = CPCodeChanger::UTF8ToUnicode(strAccount.c_str());
                }
                if (sqlite3_column_int(pStmt, 5)) {
                    ++fileTransferCount;
                    sqlite3_stmt * stmtInfo = NULL;
                    std::string sql = "select filename, total_bytes from attachment";
                    if (SQLITE_OK != sqlite3_prepare(db, sql.c_str(), -1, &stmtInfo, NULL)) {
                        continue;
                    }
                    //attachment表格中的记录顺序和message表中的文件传输消息对应
                    int i = 0;
                    while (SQLITE_ROW == sqlite3_step(stmtInfo)) {
                        if (++i == fileTransferCount) {
                            std::string filename = (const char*)sqlite3_column_text(stmtInfo, 0);
                            std::string totalbytes = (const char*)sqlite3_column_text(stmtInfo, 1);
                            pCurrentMsg->m_wstrMsg = L"[文件]: " + CPCodeChanger::UTF8ToUnicode((filename + " " + totalbytes).c_str());
                        }
                    }
                }

                m_dequeMessage.push_back(pCurrentMsg);
            }
            sqlite3_finalize(pStmt);
        }
        catch (...) {
            return false;
        }
        return true;
    }

    void CPImessageAnalyzer::Clear()
    {
        if (!m_dequeAccount.empty()) {
            for (std::deque<CPIMAccount *>::iterator it = m_dequeAccount.begin();
                 it != m_dequeAccount.end(); ++it) {
                if (NULL == *it) {
                    continue;
                }
                delete *it;
                *it = NULL;
            }
            m_dequeAccount.clear();
        }
        if (!m_dequeMessage.empty()) {
            for (std::deque<CPIMMessage *>::iterator it = m_dequeMessage.begin();
                 it != m_dequeMessage.end(); ++it) {
                if (NULL == *it) {
                    continue;
                }
                delete *it;
                *it = NULL;
            }
            m_dequeMessage.clear();
        }
    }
}
