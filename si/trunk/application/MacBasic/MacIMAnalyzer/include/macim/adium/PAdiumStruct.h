#ifndef PADIUMSTRUCT_H
#define PADIUMSTRUCT_H

enum ADIUM_PROTOCOL
{
    ADIUM_QQ        =       0x00,
    ADIUM_MSN       =       0x01,
    ADIUM_ICQ       =       0x03,
    ADIUM_AOL       =       0x04,               // AOL Instant Messager
    ADIUM_GTALK     =       0x05,               // Google Talk
    ADIUM_YAHOO     =       0x06,               // Yahoo! Message
    ADIUM_FB        =       0x07,               // FaceBook
    ADIUM_MSI       =       0x08,               // MySpaceIm
    ADIUM_LOTUS     =       0x09,               // Lotus Sametime
    ADIUM_UNKNOW	=		0xff,				// unknow
};

struct CPAccount {
    std::wstring m_wstrAccount;     // 账号名字
    std::wstring m_wstrAlias;       // 账号别名
    ADIUM_PROTOCOL m_adiumProtocol; // 使用的协议
};

/*
bool operator== (const CPAccount & lhs, const CPAccount & rhs) // 判断账号是否相同即可
{
    if (lhs.m_wstrAccount == rhs.m_wstrAccount)
    {
        return true;
    }

    return false;
}
*/

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

/*
bool operator== (const CPMsgRecord & lhs, const CPMsgRecord & rhs)
{
    if (lhs.m_wstrTime == rhs.m_wstrTime)
    {
        return true;
    }

    return false;
}

bool operator< (const CPMsgRecord & lhs, const CPMsgRecord & rhs)
{
    if (lhs.m_wstrTime < rhs.m_wstrTime)
    {
        return true;
    }

    return false;
}
*/

#endif // PADIUMSTRUCT_H
