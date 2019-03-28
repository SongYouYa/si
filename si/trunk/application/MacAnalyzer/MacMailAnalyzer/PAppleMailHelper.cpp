#include "stdafx.h"
#include "PAppleMailHelper.h"
#include "PAppleMailAnalyzer.h"
#include <QString>

#define     MAIL_DIR  L"/Library/Mail/"
#define     ACCOUNT_FILE L"Accounts.plist"
#define     CONTACT_FILE L"Envelope Index"
#define     MAIL_EXTNAME L".emlx"


using namespace filesystem;
namespace macbasic {

    CPAppleMailHelper::CPAppleMailHelper()
    {
    }
    bool CPAppleMailHelper::GetAccountRecords(std::vector<MailUser> &vecContactRecords)
    {
        std::vector<PSIHANDLE> vecFileHandles;
        //get file Handle
        MacCommonHelper::GetHandlesByUserRawPath(MAIL_DIR, ACCOUNT_FILE, vecFileHandles);
        siutil::CPHandleExport che;
        std::vector<PSIHANDLE>::iterator iterFile = vecFileHandles.begin();
        for (; iterFile != vecFileHandles.end(); ++iterFile)
        {
            std::wstring wstrFilepath = che.Export(*iterFile);
            CPAppleMailAnalyzer analyzer;
            analyzer.GetAppleMailAccounts(wstrFilepath, vecContactRecords);
        }
        return true;
    }

    bool CPAppleMailHelper::GetContactRecords(std::vector<MailUser> &vecContactRecords)
    {
        std::vector<std::wstring> vecFilePaths;
        //get export file path
        siutil::CPMacFileExport fileExport(MAIL_DIR, CONTACT_FILE);
        fileExport.GetExportFiles(vecFilePaths);
        for(auto &filepath :vecFilePaths){
            CPAppleMailAnalyzer analyzer;
            analyzer.GetAppleMailContacts(filepath, vecContactRecords);
        }
        return true;
    }
    bool CPAppleMailHelper::GetMails(std::vector<Mail> &vecMailRecords)

    {
        std::vector<PSIHANDLE> vecFileHandles;
        //get file Handle
        MacCommonHelper::GetHandlesByUserRawPath(MAIL_DIR, MAIL_EXTNAME, vecFileHandles);
        std::vector<PSIHANDLE>::iterator iterFile = vecFileHandles.begin();
        for (; iterFile != vecFileHandles.end(); ++iterFile)
        {
            CPAppleMailAnalyzer analyzer;
            Mail mailRecord;
            if(analyzer.ParseEMLX(*iterFile, mailRecord))
            {
                std::wstring wstrFilePath = CFileSystem::GetItemPath(*iterFile);
//                std::wstring strMailType = GetMailType(wstrFilePath);
//                mailRecord.m_strEmailType = strMailType;
                mailRecord.SetEmailType(wstrFilePath);
                vecMailRecords.push_back(mailRecord);
            }

        }
        return true;
    }

}
