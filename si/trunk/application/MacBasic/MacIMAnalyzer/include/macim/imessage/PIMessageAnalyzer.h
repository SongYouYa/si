#ifndef PIMESSAGEANALYZER_H
#define PIMESSAGEANALYZER_H

#include <deque>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "MacIMAnalyzer/include/macim/IMStruct.h"
#define MAC_BASE_TIME 978307200

namespace macbasic {
class CPIMessageAnalyzer
{
public:
    CPIMessageAnalyzer();
    ~CPIMessageAnalyzer();

public:
    CPIMAccount * FirstChildAccount();
    CPIMAccount * NextChildAccount();

    bool AddPlist(const std::wstring & wstrFile);
    bool PlistAnalyzer(const std::wstring & wstrFile);

    CPIMMessage * FirstChildMsg();
    CPIMMessage * NextChildMsg();

    bool AddMessage(const std::wstring & wstrFile);
    bool MessageAnalyzer(const std::wstring & wstrFile);

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
