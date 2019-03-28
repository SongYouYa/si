#ifndef _PADIUMANALYZER_HEADER_
#define _PADIUMANALYZER_HEADER_

#include "MacIMAnalyzer/include/macim/IMStruct.h"
#include <deque>

class QDomElement;

namespace macbasic {
class CPAdiumAnalyzer
{
public:
    CPAdiumAnalyzer(void);
    ~CPAdiumAnalyzer(void);

public:
    /**
            * @brief	第一个子节点
            * @param	[in]:	void
            * @return	const CPAccount *
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    CPAccount * FirstChildAccount(); // 第一个账号

    /**
            * @brief	下一个子节点
            * @param	[in]:	void
            * @return	const CPAccount *
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    CPAccount * NextChildAccount();

    /**
            * @brief	添加一个accounts文件
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddAccounts(std::wstring wstrFile);

    /**
            * @brief	accounts 文件分析器
            * @param	[in]:	const std::wstring &
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AccountsAnalyzer(const std::wstring & wstrFile);

    CPContact * FirstChildContact();

    CPContact * NextChildContact();

    /**
            * @brief	添加一个blist 文件
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddBlist(std::wstring wstrFile);

    /**
            * @brief	Blist 文件分析器
            * @param	[in]:	const std::wstring &
            * @return	int
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool BlistAnalyzer(const std::wstring & wstrFile);

    CPMsgRecord * FirstChildMsg();

    CPMsgRecord * NextChildMsg();

    /**
             * @name    AddMessage
             * @brief   添加一个聊天记录文件
             * @param   [in]:   std::wstring
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool AddMessage(std::wstring wstrFile);

    /**
            * @brief	添加一个chatlog 文件
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddChatlog(std::wstring wstrFile);

    /**
             * @name    AddHtmllog
             * @brief   添加新版本的聊天记录文件 for html
             * @param   [in]:   std::wstring
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool AddHtmllog(const std::wstring & wstrFile);

    /**
            * @brief	chatlog 文件分析器
            * @param	[in]:	const std::wstring &
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool ChatlogAnalyzer(const std::wstring & wstrFile);


    /**
             * @name    HtmllogAnalyzer
             * @brief   新版本adium的聊天记录分析
             * @param   [in]:   const std::wstring &
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool HtmllogAnalyzer(const std::wstring & wstrFile);

    /*
             * 清理函数
             */
    void Clear();

    /*
             * trim
             */
    std::wstring trim(const std::wstring & wstrIn);

    /*
             * 消息时间处理函数
             */
    time_t ProcessMsgTime(const std::wstring & wstrFile, const std::string & strTime);

private:
    /*
            * 帐号当前使用节点
            */
    std::deque<CPAccount *>::const_iterator m_AccountCurrent;

    /*
             * 帐号列表
             */
    std::deque<CPAccount *> m_dequeAccount;

    /*
             * 帐号文件列表
             */
    std::deque<std::wstring> m_dequeAccounts;

    /*
            * 联系人当前使用节点
            */
    std::deque<CPContact *>::const_iterator m_ContactCurrent;

    /*
             * 联系人列表
             */
    std::deque<CPContact *> m_dequeContact;

    /*
             * 联系人文件列表
             */
    std::deque<std::wstring> m_dequeBlist;

    /*
             * 聊天记录当前使用节点
             */
    std::deque<CPMsgRecord *>::const_iterator m_MsgCurrent;

    /*
             * 聊天记录列表
             */
    std::deque<CPMsgRecord *> m_dequeMsg;

    /*
             * 聊天记录文件列表
             */
    std::deque<std::wstring> m_dequeChatlog;



};
}
#endif
