#ifndef MAC_IMSTRUCT_H
#define MAC_IMSTRUCT_H
#include <string>
class CPTime;
namespace macbasic {
    enum SkypeRecordType{
        Skype_Account,
        Skype_Contact,
        Skype_ChatMsg,
        Skype_CallMsg,
        Skype_TransferMsg,
        Skype_Video
    };
    struct SkypeUserRecord
    {
        std::wstring	m_strUserName;			// UserName
        std::wstring	m_strDisplayName;		// Display name
        std::wstring	m_strSignName;			// 个性签名
        std::wstring	m_strProvince;			// Province
        std::wstring	m_strCity;				// City
        std::wstring	m_strCountryCode;		// Country code(ISO code)
        std::wstring	m_strPhoneNumber;		// Phone number
        std::wstring	m_strOfficeNumber;		// Office number
        std::wstring	m_strMobileNumber;		// Mobile number
        std::wstring	m_strEmail;				// Email
        std::wstring	m_strMainHome;			// 个人主页
        std::wstring	m_strSex;				// 性别
        std::wstring    m_strbirthday;          //生日
        std::wstring	m_strAbout;             // 个人说明
    };
    struct SkypeContactRecord
    {
        struct SkypeUserRecord  m_accountInfo;  // 帐号信息
        std::wstring            m_strBelong;    // 所属帐号
    };
    struct SkypeMsgRecord
    {
        std::wstring    m_strUserName;          //用户名
        SkypeRecordType m_iMsgType;           //消息类型
        std::wstring    m_strSender;            //发送者
        std::wstring    m_strReceiver;          //接受者
        std::wstring    m_strContent;           //聊天内容

        CPTime    m_iBeginTime;           //开始时间
        CPTime    m_iEndTime;            //结束时间
        //文件传输

        std::wstring    m_strFilePath;          //文件路径
        std::wstring    m_strFileName;          //文件名称
        std::wstring    m_strFileSize;          //文件大小


    };


#define ADIUM_QQ        L"QQ"
#define ADIUM_MSN       L"MSN"
#define ADIUM_ICQ       L"ICQ"
#define ADIUM_AOL       L"AOL Instant Messager"
#define ADIUM_GTALK     L"Google Talk"
#define ADIUM_YAHOO     L"Yahoo"
#define ADIUM_FB        L"FaceBook"
#define ADIUM_MSI       L"MySpaceIm"
#define ADIUM_LOTUS     L"Lotus Sametime"
#define ADIUM_UNKNOW    L""

    struct CPAccount {
        std::wstring m_wstrAccount;     // 账号名字
        std::wstring m_wstrAlias;       // 账号别名
        std::wstring m_adiumProtocol;   // 使用的协议
    };

    struct CPContact {
        std::wstring m_wstrAccount;     // 账号名称
        std::wstring m_wstrAlias;       // 账号别名
        std::wstring m_wstrGroupName;   // 所属组
        std::wstring m_wstrBelongs;     // 所属账号
    };

    struct CPMsgRecord {
        std::wstring m_wstrSender;      // 消息发送者
        std::wstring m_wstrReceiver;    // 消息接收者
        std::wstring m_wstrTime;        // 消息的时间
        std::wstring m_wstrMsgText;     // 消息内容
    };

    struct CPIMAccount {
        std::wstring        m_wstrAccount;
        std::wstring        m_wstrType;
        std::wstring        m_wstrBelongsSystem;
    };

    struct CPIMMessage {
        std::wstring m_wstrSender;
        std::wstring m_wstrReceiver;
        std::wstring m_wstrMsg;
        CPTime m_CPTime;
    };

}
#endif // IMSTRUCT_H
