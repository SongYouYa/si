#include "stdafx.h"
#include "PThunderBirdAnalyzer.h"
#include "PMail.h"
#include "filesystemshell/FileSystemShell.h"

#define THunderMark  "Mozilla"
using namespace filesystem;
namespace macbasic {
    CPThunderBirdAnalyzer::CPThunderBirdAnalyzer()
    {
    }

    bool CPThunderBirdAnalyzer::GetThunderbirdContacts(const std::wstring &wstrDBName, std::vector<MailUser> &vecContactRecords)
    {
        //打开数据库
        if (wstrDBName.empty())
        {
            return false;
        }
        sqlite3 * db = NULL;
        int iResult = 0;
        char szDBName[256];
        try {
            wcstombs(szDBName, wstrDBName.c_str(), wstrDBName.length() + 1);
            iResult = sqlite3_open(szDBName, &db);
            if (iResult)
            {
                fprintf(stderr, "Open database failed:\t%s\n", sqlite3_errmsg(db));
                sqlite3_close(db);
                return false;
            }
            else
            {
        #ifdef _DEBUG
                printf("Open database successfully!\n");
        #endif
            }
        }

        catch(...)
        {
            return false;
        }
        if(!ParseMailContacts(db, vecContactRecords))
        {
            return false;
        }
        sqlite3_close(db);
        return true;
    }

    bool CPThunderBirdAnalyzer::ParseEMLX(PSIHANDLE &fileHandle, std::vector<Mail> &vecMailRecords)
    {
        std::wstring wstrFilePath = CFileSystem::GetItemPath(fileHandle);
        uint64 length = CFileSystem::GetItemSize(fileHandle);
        uint readSize = length;
//        uint maxSize = 10 * 1024 * 1024;
//        if (length < 10 * 1024 * 1024)
//            readSize = (uint)length;
        byte * buf = new byte[readSize];
        uint64 offset = 0;

//        if (length - offset < maxSize)
//        {
//            readSize = (uint)(length - offset);
//        }
        CFileSystem::ReadFile(fileHandle, offset, readSize, buf);

        std::string strSrc((const char *)buf, readSize);
        if (strSrc.empty())
        {
            delete [] buf;
            return false;
        }
        //第一标识
        size_t prePos = strSrc.find("From - ");
        if (prePos == std::string::npos)
        {
            delete [] buf;
            return false;
        }
        QString qstrSrc = QString::fromStdString(strSrc);
        int iCount = qstrSrc.count("From - ");
        std::string strHeader = strSrc.substr(0, 256);
        if (strHeader.find(THunderMark) == std::string::npos)
        {
            delete [] buf;
            return false;
        }

//        std::string strTempSrc = strSrc.substr(prePos+7);

        //第二标识
        size_t pos = strSrc.find("From - ", 7);

        while (1)
        {
            std::string strMSrc;
            if (pos == std::string::npos)
            {
                strMSrc = strSrc.substr();
            }
            else
            {
                strMSrc = strSrc.substr(0, pos);
            }

           CPMail pMail;
           Mail mailRecord;
           if (pMail.Load(strMSrc.data(), strMSrc.length()))
           {
               mailRecord.m_strSubject = pMail.GetSubject();
               mailRecord.m_strFrom = pMail.GetFrom();
               mailRecord.m_strTo = pMail.GetTo();
               mailRecord.m_strSendTime = pMail.GetSentTime();
               mailRecord.m_strReceiveTime = pMail.GetReceiveTime();
               mailRecord.m_strAttachment = pMail.GetAttachment();
               mailRecord.m_strCC = pMail.GetCc();
               mailRecord.m_strBCC = pMail.GetBcc();
               std::wstring wstrFilePath = CFileSystem::GetItemPath(fileHandle);

               mailRecord.SetEmailType(wstrFilePath);
               vecMailRecords.push_back(mailRecord);
           }

           if (pos == std::string::npos)
           {
               offset += readSize;
               break;
           }
           strSrc = strSrc.substr(pos);
           pos = strSrc.find("From - ", 7);
        }
        delete [] buf;

        return true;
    }

    bool CPThunderBirdAnalyzer::ParseMailContacts(sqlite3 *db, std::vector<MailUser> &vecContactRecords)
    {
        if(!db)
        {
            return false;
        }

        sqlite3_stmt * pStmt = NULL;
        int iRet = sqlite3_prepare(db, "select identities.value, contacts.name from identities left join contacts on identities.contactID = contacts.id", -1, &pStmt, NULL);
        if (SQLITE_OK != iRet)
        {
            fprintf(stderr, "Query");
            return false;
        }
        while (SQLITE_ROW == sqlite3_step(pStmt))
        {
            std::string strAddress  = (const char*)sqlite3_column_text(pStmt, 0);
            std::string strName = (const char*)sqlite3_column_text(pStmt, 1);
            MailUser contactRecords;
            contactRecords.m_strUserName = CPCodeChanger::UTF8ToUnicode(strName.c_str());
            contactRecords.m_strMailAddress = CPCodeChanger::UTF8ToUnicode(strAddress.c_str());
            vecContactRecords.push_back(contactRecords);
        }
        return true;
    }
}
