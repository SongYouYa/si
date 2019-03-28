#ifndef THUNDERBIRDHELPER_H
#define THUNDERBIRDHELPER_H

#include "PMailStruct.h"
#include "PMailTaskHelper.h"

namespace macbasic {
    class CPThunderBirdHelper: public CPMailTaskHelper
    {
    public:
        CPThunderBirdHelper();
    protected:

        //===========================================================
        // brief :		获取邮件文件并解析邮件文件
        // Returns:
        // Parameter:
        // Author:    	jiaowei
        // DATE:      	2015.8.26 15:26:18
        //===========================================================
        bool GetMails(std::vector<Mail> &vecMailRecords)override;

        //===========================================================
        // brief :		获取所有联系人记录
        // Returns:
        // Parameter:	vecContactRecords	联系人记录
        // Author:    	jiaowei
        // DATE:      	2015.8.10 17:25:38
        //===========================================================
        bool GetContactRecords(std::vector<MailUser> &vecContactRecords)override;
    };
}
#endif // THUNDERBIRDHELPER_H
