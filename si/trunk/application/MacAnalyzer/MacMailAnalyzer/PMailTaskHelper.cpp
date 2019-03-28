#include "stdafx.h"
#include "PMailTaskHelper.h"

namespace macbasic {
    CPMailTaskHelper::CPMailTaskHelper()
    {
    }

    bool CPMailTaskHelper::AnalyzerMailAccount(CPRecord* rootRecord)
    {
        std::vector<MailUser> vecAccountRecords;
        if (!GetAccountRecords(vecAccountRecords))
        {
            return false;
        }

        for (std::vector<MailUser>::iterator iter = vecAccountRecords.begin();
             iter != vecAccountRecords.end(); ++iter )
        {
            CPRecord* record = rootRecord->addChild();
            record->at( L"UserName") = (*iter).m_strUserName;
            record->at( L"AccountType") = (*iter).m_strAccountType;
            record->at( L"EmailAddress") = (*iter).m_strMailAddress;
            record->at( L"HostName") = (*iter).m_strHostName;
            record->at(L"Describe") = (*iter).m_strAppType;
        }
        return true;
    }

    bool CPMailTaskHelper::AnalyzerMailContact(CPRecord* rootRecord)
    {
        std::vector<MailUser> vecContactRecords;
        if (!GetContactRecords(vecContactRecords))
        {
            return false;
        }
        for (std::vector<MailUser>::iterator iter = vecContactRecords.begin();
             iter != vecContactRecords.end(); ++iter )
        {

            CPRecord* record = rootRecord->addChild();
            record->at(L"UserName") = (*iter).m_strUserName;
            record->at(L"EmailAddress") = (*iter).m_strMailAddress;

        }
        return true;
    }

    bool CPMailTaskHelper::AnalyzerMailRecords(CPRecord* rootRecord)
    {
        std::vector<Mail> vecMailRecords;
        if (!GetMails(vecMailRecords))
        {
            return false;
        }


        for (std::vector<Mail>::iterator iter = vecMailRecords.begin();
             iter != vecMailRecords.end(); ++iter )
        {

          CPRecord* record = rootRecord->addChild();
            record->at(L"Subject") = (*iter).m_strSubject;
            record->at(L"FromName") = (*iter).m_strFrom;
            record->at(L"ToName") = (*iter).m_strTo;
            record->at(L"SendTime") = (*iter).m_strSendTime;
            record->at(L"ReceiveTime") = (*iter).m_strReceiveTime;
            record->at(L"Attachment") = (*iter).m_strAttachment;
            record->at(L"CC") = (*iter).m_strCC;
            record->at(L"BCC") = (*iter).m_strBCC;
            record->at(L"EmailType") = (*iter).m_strEmailType;
        }
        return true;
    }
    bool CPMailTaskHelper::GetAccountRecords(std::vector<MailUser> &vecContactRecords)
    {
        return true;
    }

    bool CPMailTaskHelper::GetContactRecords(std::vector<MailUser> &vecContactRecords)
    {
        return true;
    }
    bool CPMailTaskHelper::GetMails(std::vector<Mail> &vecMailRecords)
    {
        return true;
    }

}
