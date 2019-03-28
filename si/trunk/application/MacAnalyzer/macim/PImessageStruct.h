#ifndef IMESSAGESTRUCT_H
#define IMESSAGESTRUCT_H

#include "base/PTime.h"

struct CPIMAccount {
    std::wstring  m_wstrAccount;
    std::wstring  m_wstrType;
    std::wstring   m_wstrBelongsSystem;
};

struct CPIMMessage {
    std::wstring m_wstrSender;
    std::wstring m_wstrReceiver;
    std::wstring m_wstrMsg;
    CPTime m_CPTime;
};

#endif // IMESSAGESTRUCT_H
