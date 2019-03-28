#ifndef MAC_MAILSTRUCT_H
#define MAC_MAILSTRUCT_H

#include "base/PTime.h"
#include <QString>
namespace macbasic {

    enum EPRecordType
    {
    MAIL_NONE		= 0x1,
    MAIL_MAIL		= 0x2,
    MAIL_USER		= 0x4,
    MAIL_CONTACT    = 0x8,
    MAIL_RSS 		= 0x10,
    };

    enum EPMailType
    {
        MAIL_FOXMAIL		= 0x0001,		// foxmil
        MAIL_OE				= 0x0002,		// oe
        MAIL_OUTLOOK		= 0x0004,		// outlook
        MAIL_WEB			= 0x0008,		// webmail
        MAIL_EML		    = 0x0010,		// maildir(eml)
        MAIL_MBOX			= 0x0020,		// mbox
        MAIL_THUNDERBIRD    = 0x0040,
        MAIL_APPLEMAIL      = 0x0080,
        MAIL_ENTOURAGE        = 0x0100,
    };
    struct MailUser
    {
        std::wstring m_strMailAddress;
        std::wstring m_strUserName;
        std::wstring m_strHostName;
        std::wstring m_strAccountType;
        std::wstring m_strAppType;
    };
    struct Mail
    {
        std::wstring m_strSubject;
        std::wstring m_strFrom;
        std::wstring m_strTo;
        CPTime m_strSendTime;
        CPTime m_strReceiveTime;
        std::wstring m_strRead;
        std::wstring m_strAttachment;
        std::wstring m_strCC;
        std::wstring m_strBCC;
        std::wstring m_strEmailType;

        void SetEmailType(const std::wstring& strFilePath)
        {
            QString qstrFilePath = QString::fromStdWString(strFilePath);
            if (qstrFilePath.contains(QString("INBOX"), Qt::CaseInsensitive))
            {
                m_strEmailType = L"收件箱";
            }
            else if (qstrFilePath.contains(QString("Sent"), Qt::CaseInsensitive))
            {
                m_strEmailType = L"发件箱";
            }
            else if (qstrFilePath.contains(QString("Draft"), Qt::CaseInsensitive))
            {
                m_strEmailType = L"草稿箱";
            }
            else if (qstrFilePath.contains(QString("Trash"), Qt::CaseInsensitive))
            {
                m_strEmailType = L"垃圾邮件";
            }
            else if (qstrFilePath.contains(QString("Deleted Messages"), Qt::CaseInsensitive))
            {
                m_strEmailType = L"废纸篓";
            }
            else
            {
                m_strEmailType = L"其他";
            }
        }
    };
}
#endif // IMSTRUCT_H
