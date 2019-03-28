#ifndef THUNDERBIRDANALYZER_H
#define THUNDERBIRDANALYZER_H
extern "C" {
#include "sqlite3.h"
}
#include "PMailStruct.h"


namespace macbasic {
    class CPThunderBirdAnalyzer
    {
    public:
        CPThunderBirdAnalyzer();

        //===========================================================
        // brief :      解析邮件联系人
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.26 15:26:18
        //===========================================================
        bool GetThunderbirdContacts(const std::wstring& wstrDBName, std::vector<MailUser> &vecContactRecords);

        //===========================================================
        // brief :		解析 Mail
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.26 15:26:18
        //===========================================================
        bool ParseEMLX(PSIHANDLE& fileHandle, std::vector<Mail> &vecMailRecords);
    private:

        //===========================================================
        // brief :		分析数据库
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.26 15:26:18
        //===========================================================
        bool ParseMailContacts(sqlite3 * m_pDB, std::vector<MailUser> &vecContactRecords);
    };
}
#endif // THUNDERBIRDANALYZER_H
