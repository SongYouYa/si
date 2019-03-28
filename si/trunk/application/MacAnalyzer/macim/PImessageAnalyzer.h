#ifndef PIMESSAGEANALYZER_H
#define PIMESSAGEANALYZER_H

#include <deque>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "macim/IMStruct.h"
#define MAC_BASE_TIME 978307200

namespace macbasic {
class CPImessageAnalyzer
{
public:
    CPImessageAnalyzer();
    ~CPImessageAnalyzer();

public:
    CPIMAccount * FirstChildAccount();
    CPIMAccount * NextChildAccount();

    bool addPlist(const std::wstring & wstrFile);
    bool plistAnalyzer(const std::wstring & wstrFile);

    CPIMMessage * FirstChildMsg();
    CPIMMessage * NextChildMsg();

    bool addMessage(const std::wstring & wstrFile);
    bool messageAnalyzer(const std::wstring & wstrFile);

    void Clear();

private:
    std::deque<CPIMAccount *>::const_iterator m_itCurrent;
    std::deque<CPIMMessage *>::const_iterator m_itCurrentM;
    /*
             * 帐号列表
             */
    std::deque<CPIMAccount *> m_dequeAccount;

    /*
             * 聊天记录列表
             */
    std::deque<CPIMMessage *> m_dequeMessage;

    /*
             * 帐号文件列表
             */
    std::deque<std::wstring> m_dequePlist;

    /*
             * 聊天记录文件列表
             */
    std::deque<std::wstring> m_dequeChatDb;

};
}
#endif // PIMESSAGEANALYZER_H
