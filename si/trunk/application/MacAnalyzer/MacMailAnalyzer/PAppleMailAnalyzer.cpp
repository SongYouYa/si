#include "stdafx.h"
#include "PAppleMailAnalyzer.h"
#include "PMail.h"

using namespace filesystem;
namespace macbasic{

    CPAppleMailAnalyzer::CPAppleMailAnalyzer()
    {
    }
    bool CPAppleMailAnalyzer::GetAppleMailAccounts(const std::wstring& strPath, std::vector<MailUser> &vecAccountRecords)
    {
        plistcpp::dictionary_type dictData;
        std::string strPlistPath = CPCodeChanger::UnicodeToUTF8(strPath.c_str());
        try {
            plistcpp::CPlistCpp::ReadPlist(strPlistPath.c_str(), dictData);
        }
        catch(...)
        {
            return false;
        }
        plistcpp::dictionary_type::iterator itrDict = dictData.find("MailAccounts");
        if(itrDict == dictData.end())
        {
            return false;
        }
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            MailUser accountRecord;
            plistcpp::dictionary_type AccountData = boost::any_cast<plistcpp::dictionary_type>(*iterArray);
            //EmailAddresses
            plistcpp::dictionary_type::iterator iterAccount = AccountData.find("EmailAddresses");
            if(iterAccount == AccountData.end())
            {
                continue;
            }
            plistcpp::array_type arrayAddresses = boost::any_cast<plistcpp::array_type>(iterAccount->second);
            plistcpp::array_type::iterator iteraddress = arrayAddresses.begin();
            std::string strEmailAddresses;
            for(; iteraddress != arrayAddresses.end(); ++iteraddress)
            {
                if (!strEmailAddresses.empty())
                {
                    strEmailAddresses += ";";
                }
                strEmailAddresses += boost::any_cast<plistcpp::string_type>(*iteraddress);
            }
            accountRecord.m_strMailAddress = CPCodeChanger::UTF8ToUnicode(strEmailAddresses.c_str());
            //AccountType
            iterAccount = AccountData.find("AccountType");
            if(iterAccount != AccountData.end())
            {
                std::string strAccountType = boost::any_cast<plistcpp::string_type>(iterAccount->second);
                accountRecord.m_strAccountType = CPCodeChanger::UTF8ToUnicode(strAccountType.c_str());
            }
            //Hostname
            iterAccount = AccountData.find("Hostname");
            if(iterAccount != AccountData.end())
            {
                std::string strHostName = boost::any_cast<plistcpp::string_type>(iterAccount->second);
                accountRecord.m_strHostName = CPCodeChanger::UTF8ToUnicode(strHostName.c_str());
            }
            std::string strHostName = boost::any_cast<plistcpp::string_type>(iterAccount->second);
            accountRecord.m_strHostName = CPCodeChanger::UTF8ToUnicode(strHostName.c_str());
            //Username
            iterAccount = AccountData.find("Username");
            if(iterAccount != AccountData.end())
            {
                std::string strUserName = boost::any_cast<plistcpp::string_type>(iterAccount->second);
                accountRecord.m_strUserName = CPCodeChanger::UTF8ToUnicode(strUserName.c_str());
            }
            //AccountName
            iterAccount = AccountData.find("AccountName");
            if(iterAccount != AccountData.end())
            {
                std::string strAppType = boost::any_cast<plistcpp::string_type>(iterAccount->second);
                accountRecord.m_strAppType = CPCodeChanger::UTF8ToUnicode(strAppType.c_str());
            }
            vecAccountRecords.push_back(accountRecord);
        }
        return true;
    }

    bool CPAppleMailAnalyzer::GetAppleMailContacts(const std::wstring& wstrDBName, std::vector<MailUser> &vecContactRecords)
    {
        boost::mutex::scoped_lock lock(mutex_);
        //打开数据库
        if (wstrDBName.empty()){
            return false;
        }
        sqlite3 * db = NULL;
        int iResult = 0;
        char szDBName[256];
        try {
            wcstombs(szDBName, wstrDBName.c_str(), wstrDBName.length() + 1);
            iResult = sqlite3_open(szDBName, &db);
            if (iResult){
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
        if (!ParseMailContacts(db, vecContactRecords))
        {
            return false;
        }
        sqlite3_close(db);
        return true;
    }
    bool CPAppleMailAnalyzer::ParseMailContacts(sqlite3 * db, std::vector<MailUser> &vecContactRecords)
    {
        if(!db)
        {
            return false;
        }

        sqlite3_stmt * pStmt = NULL;
        int iRet = sqlite3_prepare(db, "select address, comment from addresses", -1, &pStmt, NULL);
        if (SQLITE_OK != iRet)
        {
            fprintf(stderr, "Query");
            return false;
        }

        while (SQLITE_ROW == sqlite3_step(pStmt))
        {

            std::string strAddress  = (const char*)sqlite3_column_text(pStmt, 0);
            std::string strComment = (const char*)sqlite3_column_text(pStmt, 1);
            uint i;
            for (i = 0; i < vecContactRecords.size(); i++)
            {
                if(wcscmp(vecContactRecords[i].m_strMailAddress.c_str(),
                          CPCodeChanger::UTF8ToUnicode(strAddress.c_str()).c_str()) == 0)
                {
                    break;
                }
            }


            if(i < vecContactRecords.size())
            {
                // find the same address
                if(!strComment.empty())
                {
                    if(!vecContactRecords[i].m_strUserName.empty())
                    {
                        vecContactRecords[i].m_strUserName.append(L"; ");
                    }
                    vecContactRecords[i].m_strUserName.append(CPCodeChanger::UTF8ToUnicode(strComment.c_str()));
                }

            }
            else // a new address
            {
                MailUser userRecords;
                userRecords.m_strMailAddress = CPCodeChanger::UTF8ToUnicode(strAddress.c_str());
                userRecords.m_strUserName = CPCodeChanger::UTF8ToUnicode(strComment.c_str());
                vecContactRecords.push_back(userRecords);
            }

        }

        sqlite3_finalize(pStmt);
        return true;
    }

    bool CPAppleMailAnalyzer::ParseEMLX(PSIHANDLE& fileHandle, Mail &mailRecord)
    {
         uint64 length = CFileSystem::GetItemSize(fileHandle);
         uint readSize = length;
//         uint maxSize = 10 * 1024 * 1024;
//         if (length < 10 * 1024 * 1024)
//             readSize = (uint)length;
         byte * buf = new byte[readSize];
         uint64 offset = 0;

//         if (length - offset < maxSize)
//         {
//             readSize = (uint)(length - offset);
//         }
         CFileSystem::ReadFile(fileHandle, offset, readSize, buf);
         std::string strSrc((const char *)buf, 100);
         size_t pos = strSrc.find("\n");
         uint nMailLength = 0;
         std::string strLength = strSrc.substr(0, pos);

         try{
             boost::trim(strLength);
             nMailLength = boost::lexical_cast<uint>(strLength);
         }
         catch(boost::bad_lexical_cast &e)
         {
             std::cout << "error:"<< e.what() << std::endl;
             delete [] buf;
             return false;
         }

         if(readSize < nMailLength + pos)
         {
             std::cout << "buffer size is too small!" << std::endl;
             delete [] buf;
             return false;
         }

         CPMail pMail;
         if (!pMail.Load((const char *)(buf + pos  + 1), (int)(nMailLength)))
         {
             delete [] buf;
             return false;
         }
         mailRecord.m_strSubject = pMail.GetSubject();
         mailRecord.m_strFrom = pMail.GetFrom();
         mailRecord.m_strTo = pMail.GetTo();
         mailRecord.m_strSendTime = pMail.GetSentTime();
         mailRecord.m_strReceiveTime = pMail.GetReceiveTime();
         mailRecord.m_strAttachment = pMail.GetAttachment();
         mailRecord.m_strCC = pMail.GetCc();
         mailRecord.m_strBCC = pMail.GetBcc();
         delete [] buf;
         return true;
    }
}
