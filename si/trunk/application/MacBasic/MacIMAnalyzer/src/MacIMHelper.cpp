#include"stdafx.h"
#include "MacIMAnalyzer/include/macim/MacIMHelper.h"
#include "MacIMAnalyzer/include/macim/skype/SkypeHelper.h"
#include "MacIMAnalyzer/include/macim/adium/AdiumTaskHelper.h"
#include "MacIMAnalyzer/include/macim/imessage/IMessageTaskHelper.h"

namespace macim {

    MacIMHelper::MacIMHelper()
    {
    }
    task::CallBack MacIMHelper::GetCallbackByTaskId(task::TaskDefId id)
    {
        switch(id)
        {
#ifdef OS_PLINUX
        case task::TaskSkypeAccount:
            {
                SkypeHelper helper;
                return boost::BOOST_BIND(&SkypeHelper::GetSkypeAccount,helper,_1,_2);//
            }
        case task::TaskSkypeFriends:
            {
                SkypeHelper helper;
                return boost::BOOST_BIND(&SkypeHelper::GetSkypeFriends,helper,_1,_2);//
            }
        case task::TaskSkypeMsg:
            {
                SkypeHelper helper;
                return boost::BOOST_BIND(&SkypeHelper::GetSkypeMsgs,helper,_1,_2);//
            }
        case task::TaskAdiumAccount:
            {
                AdiumTaskHelper helper;
                return boost::BOOST_BIND(&AdiumTaskHelper::GetAdiumAccountRecords,helper,_1,_2);//
            }
        case task::TaskAdiumFriend:
            {
                AdiumTaskHelper helper;
                return boost::BOOST_BIND(&AdiumTaskHelper::GetAdiumContactRecords,helper,_1,_2);//
            }
        case task::TaskAdiumMsg:
            {
                AdiumTaskHelper helper;
                return boost::BOOST_BIND(&AdiumTaskHelper::GetAdiumMsgRecords,helper,_1,_2);//
            }
        case task::TaskIMessageAccount:
            {
                IMessageTaskHelper helper;
                return boost::BOOST_BIND(&IMessageTaskHelper::GetIMessageAccountRecords,helper,_1,_2);//
            }
        case task::TaskIMessageMsg:
            {
                IMessageTaskHelper helper;
                return boost::BOOST_BIND(&IMessageTaskHelper::GetIMessageMsgRecords,helper,_1,_2);//
            }
#endif
        }
        return task::CallBack(NULL);
    }
}
