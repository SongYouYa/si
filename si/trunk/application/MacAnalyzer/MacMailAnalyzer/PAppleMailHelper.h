#ifndef APPLEMAILHELPER_H
#define APPLEMAILHELPER_H
#include "PMailStruct.h"
#include "PMailTaskHelper.h"

namespace macbasic {
    class CPAppleMailHelper:public CPMailTaskHelper
    {
    public:

        CPAppleMailHelper();
    protected:
        //===========================================================
        // brief :		获取所有账号记录
        // Returns:
        // Parameter:	vecAccountRecords	账号记录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
       virtual  bool GetAccountRecords(std::vector<MailUser> &vecAccountRecords)override;

        //===========================================================
        // brief :		获取所有联系人记录
        // Returns:
        // Parameter:	vecContactRecords	联系人记录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
       virtual  bool GetContactRecords(std::vector<MailUser> &vecContactRecords)override;

        //===========================================================
        // brief :		获取Apple Mail
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
       virtual bool GetMails(std::vector<Mail> &vecMailRecords)override;


    };
}
#endif // APPLEMAILHELPER_H
