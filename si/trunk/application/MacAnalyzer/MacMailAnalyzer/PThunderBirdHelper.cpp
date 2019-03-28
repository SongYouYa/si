#include "stdafx.h"
#include "PThunderBirdHelper.h"
#include "PThunderBirdAnalyzer.h"

#define THUNDERBIRD_DIR L"/Library/Thunderbird/Profiles/"
#define CONTACT_FILE    L"global-messages-db.sqlite"
#define INBOX_MAIL      L"Inbox"
#define SENT_MAIL       L"Sent"
#define TRASH_MAIL      L"Trash"
#define DRAFT_MAIL      L"Draft"
#define MAIL_FILE       L"Inbox|Sent|Trash|Draft|Deleted Messages"
using namespace filesystem;

namespace macbasic {
    CPThunderBirdHelper::CPThunderBirdHelper()
    {
    }

    bool CPThunderBirdHelper::GetContactRecords(std::vector<MailUser> &vecContactRecords)
    {
        std::vector<std::wstring> vecFilePaths;
        //get export file path
        CPMacFileExport fileExport(THUNDERBIRD_DIR, CONTACT_FILE);
        fileExport.GetExportFiles(vecFilePaths);
        std::vector<std::wstring>::iterator iterFile = vecFilePaths.begin();
        for (; iterFile != vecFilePaths.end(); ++iterFile)
        {
            CPThunderBirdAnalyzer analyzer;
            analyzer.GetThunderbirdContacts(*iterFile, vecContactRecords);
        }

        return true;
    }

    bool CPThunderBirdHelper::GetMails(std::vector<Mail> &vecMailRecords)
    {
        std::vector<PSIHANDLE> vecFileHandles;
        //get file Handle

        siutil::CFileSearch fileSearch(L"",L"", MAIL_FILE);
        fileSearch.GetHandles(vecFileHandles);
//        MacCommonHelper::GetHandlesByUserRawPath(THUNDERBIRD_DIR, INBOX_MAIL, vecFileHandles);
//        MacCommonHelper::GetHandlesByUserRawPath(THUNDERBIRD_DIR, SENT_MAIL, vecFileHandles);
//        MacCommonHelper::GetHandlesByUserRawPath(THUNDERBIRD_DIR, TRASH_MAIL, vecFileHandles);
//        MacCommonHelper::GetHandlesByUserRawPath(THUNDERBIRD_DIR, DRAFT_MAIL, vecFileHandles);


        std::vector<PSIHANDLE>::iterator iterFile = vecFileHandles.begin();
        for (; iterFile != vecFileHandles.end(); ++iterFile)
        {
            CPThunderBirdAnalyzer analyzer;
            if(!analyzer.ParseEMLX(*iterFile, vecMailRecords))
            {
                continue;
            }

        }
        return true;

    }
}
