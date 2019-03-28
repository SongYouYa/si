#ifndef MAC_APPLEMAILANALYZER_H
#define MAC_APPLEMAILANALYZER_H

extern "C" {
#include "sqlite3.h"
}
#include "PMailStruct.h"
#include <boost/thread/mutex.hpp>

namespace macbasic{

    class CPAppleMailAnalyzer
    {
    public:
        CPAppleMailAnalyzer();

        bool GetAppleMailAccounts(const std::wstring& strPath, std::vector<MailUser> &vecAccountRecords);

        bool GetAppleMailContacts(const std::wstring& wstrDBName, std::vector<MailUser> &vecContactRecords);

        bool ParseEMLX(PSIHANDLE& fileHandle, Mail &mailRecord);
    private:

        void Close();

        bool ParseMailContacts(sqlite3 * db, std::vector<MailUser> &vecContactRecords);
    private:
        boost::mutex mutex_;

    };

}

#endif // APPLEMAILANALYZER_H
