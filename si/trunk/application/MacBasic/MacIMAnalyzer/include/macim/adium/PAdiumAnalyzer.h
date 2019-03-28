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
            * @brief	��һ���ӽڵ�
            * @param	[in]:	void
            * @return	const CPAccount *
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    CPAccount * FirstChildAccount(); // ��һ���˺�

    /**
            * @brief	��һ���ӽڵ�
            * @param	[in]:	void
            * @return	const CPAccount *
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    CPAccount * NextChildAccount();

    /**
            * @brief	���һ��accounts�ļ�
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddAccounts(std::wstring wstrFile);

    /**
            * @brief	accounts �ļ�������
            * @param	[in]:	const std::wstring &
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AccountsAnalyzer(const std::wstring & wstrFile);

    CPContact * FirstChildContact();

    CPContact * NextChildContact();

    /**
            * @brief	���һ��blist �ļ�
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddBlist(std::wstring wstrFile);

    /**
            * @brief	Blist �ļ�������
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
             * @brief   ���һ�������¼�ļ�
             * @param   [in]:   std::wstring
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool AddMessage(std::wstring wstrFile);

    /**
            * @brief	���һ��chatlog �ļ�
            * @param	[in]:	std::wstring
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool AddChatlog(std::wstring wstrFile);

    /**
             * @name    AddHtmllog
             * @brief   ����°汾�������¼�ļ� for html
             * @param   [in]:   std::wstring
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool AddHtmllog(const std::wstring & wstrFile);

    /**
            * @brief	chatlog �ļ�������
            * @param	[in]:	const std::wstring &
            * @return	bool
            * @author	nct(xgs@pansafe.com)
            * @date	2009.06.01
            */
    bool ChatlogAnalyzer(const std::wstring & wstrFile);


    /**
             * @name    HtmllogAnalyzer
             * @brief   �°汾adium�������¼����
             * @param   [in]:   const std::wstring &
             * @return  bool
             * @author  Jesse.Xu (xgs), xgs@pansafe.com
             */
    bool HtmllogAnalyzer(const std::wstring & wstrFile);

    /*
             * ������
             */
    void Clear();

    /*
             * trim
             */
    std::wstring trim(const std::wstring & wstrIn);

    /*
             * ��Ϣʱ�䴦����
             */
    time_t ProcessMsgTime(const std::wstring & wstrFile, const std::string & strTime);

private:
    /*
            * �ʺŵ�ǰʹ�ýڵ�
            */
    std::deque<CPAccount *>::const_iterator m_AccountCurrent;

    /*
             * �ʺ��б�
             */
    std::deque<CPAccount *> m_dequeAccount;

    /*
             * �ʺ��ļ��б�
             */
    std::deque<std::wstring> m_dequeAccounts;

    /*
            * ��ϵ�˵�ǰʹ�ýڵ�
            */
    std::deque<CPContact *>::const_iterator m_ContactCurrent;

    /*
             * ��ϵ���б�
             */
    std::deque<CPContact *> m_dequeContact;

    /*
             * ��ϵ���ļ��б�
             */
    std::deque<std::wstring> m_dequeBlist;

    /*
             * �����¼��ǰʹ�ýڵ�
             */
    std::deque<CPMsgRecord *>::const_iterator m_MsgCurrent;

    /*
             * �����¼�б�
             */
    std::deque<CPMsgRecord *> m_dequeMsg;

    /*
             * �����¼�ļ��б�
             */
    std::deque<std::wstring> m_dequeChatlog;



};
}
#endif
