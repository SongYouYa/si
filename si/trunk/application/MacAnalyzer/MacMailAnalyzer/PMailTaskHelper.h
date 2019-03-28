#ifndef MAILTASKHELPER_H
#define MAILTASKHELPER_H
#include "PMailStruct.h"

namespace macbasic {
    class CPMailTaskHelper
    {
    public:
//        enum MailUserType
//        {
//            Mail_Account,
//            Mail_Contact
//        };
        CPMailTaskHelper();
        //===========================================================
        // brief :		具体解析邮件账号
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        virtual bool AnalyzerMailAccount(CPRecord* rootRecord);

        //===========================================================
        // brief :		具体解析邮件联系人
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.12 14:25:38
        //===========================================================
        virtual bool AnalyzerMailContact(CPRecord* rootRecord);

        //===========================================================
        // brief :		解析Apple Mail
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        virtual bool AnalyzerMailRecords(CPRecord* rootRecord);

        //===========================================================
        // brief :		获取所有账号记录
        // Returns:
        // Parameter:	vecAccountRecords	账号记录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        virtual bool GetAccountRecords(std::vector<MailUser> &vecAccountRecords);

        //===========================================================
        // brief :		获取所有联系人记录
        // Returns:
        // Parameter:	vecContactRecords	联系人记录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        virtual bool GetContactRecords(std::vector<MailUser> &vecContactRecords);

        //===========================================================
        // brief :		获取Apple Mail
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        virtual bool GetMails(std::vector<Mail> &vecMailRecords);



    };
}
#endif // MAILTASKHELPER_H
