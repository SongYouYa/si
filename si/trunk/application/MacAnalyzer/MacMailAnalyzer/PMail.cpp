#include "stdafx.h"
#include "PMime.h"
#include "PMimeCode.h"
#include "siutility/PCodeConverter.h"

#include "PMail.h"
#include "MacMailAnalyzer/PAppleMailAnalyzer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
namespace macbasic
{

    CPMail::CPMail(void) : m_bRead(false)
    {
//        m_type = CPEntry::ENTRY_TYPE_MAIL;
        m_eType = MAIL_EML;
        m_nMark = -1;
        m_bRead = true;

        m_epRecordType = MAIL_MAIL;
        
    }

    CPMail::~CPMail(void)
    {
    }


    std::wstring CPMail::GetFrom() const
    {
        return m_strFrom;
    }


    std::wstring CPMail::GetTo() const
    {
        return m_strTo;
    }


    std::wstring CPMail::GetCc() const
    {
        return m_strCc;
    }

    std::wstring CPMail::GetBcc() const
    {
        return m_strBcc;
    }


    CPTime CPMail::GetSaveTime() const
    {
        return m_tSave;
    }


    CPTime CPMail::GetSentTime() const
    {
        return m_tSend;
    }


    CPTime CPMail::GetReceiveTime() const
    {
        return m_tReceive;
    }

    std::wstring CPMail::GetText() const
    {
        return m_strText;
    }

    uint CPMail::GetAttachmentCount() const
    {
        return (uint)m_arrAttachment.size();
    }

    std::wstring CPMail::HtmlToText(const std::string& strHtml)
    {
        if (strHtml.empty())
            return _T("");

        std::string str = strHtml;
        std::string strText;

        // ÍÚ³öÃ¿¶Î > ... < ÖÐ¼ä²¿·ÖµÄÄÚÈÝ
        while (!str.empty())
        {

            std::string::size_type begin = str.find_first_of(">");
            if (begin == std::string::npos)
            {
                break;
            }
            str = str.substr(begin+1, str.length()-(begin-1));

            std::string::size_type end = str.find_first_of("<");
            if (end == std::string::npos)
            {
                break;
            }
            strText += str.substr(0, end);
            str = str.substr(end, str.length()-end);
        }

        std::string::size_type pos = strText.find("\r\n");
        while (pos != std::string::npos)
        {
            strText.erase(pos, 2);
            pos = strText.find("\r\n");
        }

        return CPCodeChanger::UTF8ToUnicode(strText.c_str());
    }

    bool CPMail::Load(const char* buf, int nSize)
    {
        // ¼ÓÔØÓÊ¼þ
        CMimeMessage* pMessage = new CMimeMessage();
        std::auto_ptr<CMimeMessage> pMessage_ptr(pMessage);

        int nBodyLength = pMessage->Load(buf, nSize);
        if (nBodyLength <= 0)
        {
            return false;
        }
       
  
        std::string str;
        pMessage->GetFrom(str);
        if (!str.empty())
        {
          
            size_t pos = str.find(char(32));
        
            while(std::string::npos != pos)
            {
               if(str[pos+1] == '<')
               {

                   str = str.substr(pos+1);
                   break;

               }
               pos= str.find(char(32), pos + 1);

            }

//            if((std::string::npos != pos) && (str[pos+1] == '<'))
//            {
//                str = str.substr(pos+1);
//            }
            bool bUtf8 =  CPCodeConverter::IsCodeUTF8(str.c_str());
            if (bUtf8)
            {
                m_strFrom =  CPCodeChanger::UTF8ToUnicode(str.c_str());
            }
            else
            {
                m_strFrom = CPCodeConverter::Decode(str.c_str(),str.length(), CPCodeConverter::GBK);

            }

        }
        else
        {
            pMessage->GetRSSArticleUrl(str);
            if (!str.empty())
            {                
                // find a appleMail rss artical
                this->SetType(MAIL_RSS);
                m_strFrom =  CPCodeChanger::UTF8ToUnicode(str.c_str());
            }
            
        }
        
        if (m_strFrom.empty())
        {
            return false;
        }
        pMessage->GetContentBase(str);
        if (!str.empty())
        {
            // find a thunderbird rss artical
            m_strFrom += CPCodeChanger::UTF8ToUnicode(str.c_str());
            this->SetType(MAIL_RSS);
            
        }
        
        pMessage->GetTo(str);
        if (!str.empty())
        {      
        //std::cout << "To String ::" << str << std::endl;
        size_t pos;
        pos= str.find(char(32));


        while(std::string::npos != pos)
        {
           if(str[pos+1] == '<')
           {

               str = str.substr(pos+1);
               break;

           }
           pos= str.find(char(32), pos + 1);

        }
        //std::cout << "To String ::" << str << std::endl;
        bool bUtf8 =  CPCodeConverter::IsCodeUTF8(str.c_str());
        if (bUtf8)
        {
            m_strTo =  CPCodeChanger::UTF8ToUnicode(str.c_str());
        }
        else
        {
            m_strTo = CPCodeConverter::Decode(str.c_str(), str.length(), CPCodeConverter::GBK);

        }

        }
        else
        {
            pMessage->GetDeliveredTo(str);
            if (!str.empty())
            {
                //std::cout << "DelievedTo String ::" << str << std::endl;
                size_t pos;
                pos= str.find('-');
                if(std::string::npos != pos)
                {
                    str = str.substr(pos+1);
                }
                //std::cout << "DelievedToTo String ::" << str << std::endl;

                m_strTo =  CPCodeChanger::UTF8ToUnicode(str.c_str());

            }

        }
        if (m_strTo.empty())
        {
            return false;
        }

        pMessage->GetCc(str);
        if (!str.empty())
        {
        bool bUtf8 =  CPCodeConverter::IsCodeUTF8(str.c_str());
        if (bUtf8)
        {
            m_strCc =  CPCodeChanger::UTF8ToUnicode(str.c_str());
        }
        else
        {
            m_strCc = CPCodeConverter::Decode(str.c_str(), str.length(),CPCodeConverter::GBK);

        }

        }

        pMessage->GetBcc(str);
        if (!str.empty())
        {

//      m_strBcc =  CPCodeChanger::UTF8ToUnicode(str.c_str());
        bool bUtf8 =  CPCodeConverter::IsCodeUTF8(str.c_str());
        if (bUtf8)
        {
            m_strBcc =  CPCodeChanger::UTF8ToUnicode(str.c_str());
        }
        else
        {
            m_strBcc = CPCodeConverter::Decode(str.c_str(), str.length(), CPCodeConverter::GBK);

        }
        }

        pMessage->GetSubject(str);
        if (!str.empty())
 
        {
            std::wstring wstrSubject;
           bool bUtf8 =  CPCodeConverter::IsCodeUTF8(str.c_str());
           if (bUtf8)
           {
                wstrSubject = CPCodeChanger::UTF8ToUnicode(str.c_str());
           }
           else
           {
               wstrSubject = CPCodeConverter::Decode(str.c_str(),  str.length(), CPCodeConverter::GBK);
           }
            this->SetSubject(wstrSubject);
        }

        pMessage->GetDate(str);
        if (!str.empty())
        {
            std::wstring strTime = CPCodeChanger::UTF8ToUnicode(str.c_str());
            CPTime tSent = CPTime::DecodeMailTime(strTime);
            bool bGMT = (strTime.find(_T(" -0000")) != std::wstring::npos) ||
                        (strTime.find(_T(" +0000")) != std::wstring::npos);

            if (bGMT)
            {
                tSent = CPTime::UTC2LocalTime(tSent, siutil::MacCommonHelper::GetTimezoneRegion());
            }
            this->SetSendTime(tSent);
        }

        // ÉèÖÃÓÊ¼þ·þÎñÆ÷½ÓÊÜÊ±¼ä
        pMessage->GetReceived(str);
        if (!str.empty())
        {
            std::wstring strTime = CPCodeChanger::UTF8ToUnicode(str.c_str());
            CPTime tReceive = CPTime::DecodeMailTime(strTime);
            bool bGMT = (strTime.find(_T(" -0000")) != std::wstring::npos) ||
                        (strTime.find(_T(" +0000")) != std::wstring::npos);

            if (bGMT)
            {
                tReceive = CPTime::UTC2LocalTime(tReceive, siutil::MacCommonHelper::GetTimezoneRegion());
            }
            this->SetReceiveTime(tReceive);
        }

        pMessage->GetPlainText(str);

        if (!str.empty()){

           m_strText += CPCodeConverter::Decode(str.c_str(), str.length(), CPCodeConverter::GBK);
           
        }

        if (m_strText.empty())
        {
            pMessage->GetHtmlText(str);

            if (!str.empty())
            {

                m_strText = HtmlToText(str.c_str());
            }
        }

        m_arrAttachment.clear();
        std::vector< std::pair<std::string, int> > arrAttachment;

        pMessage->GetAttachmentList(arrAttachment);
        for (uint i=0; i<arrAttachment.size(); i++)
        {

        std::wstring strName;
        bool bUtf8 =  CPCodeConverter::IsCodeUTF8(arrAttachment[i].first.c_str());
        if (bUtf8)
        {
             strName = CPCodeChanger::UTF8ToUnicode(arrAttachment[i].first.c_str());
        }
        else
        {
             strName = CPCodeConverter::Decode(arrAttachment[i].first.c_str(),
                                                 arrAttachment[i].first.length(),
                                                 CPCodeConverter::GBK);
        }
//      std::wstring strName = CPCodeChanger::UTF8ToUnicode(arrAttachment[i].first.c_str());

            int nSize = arrAttachment[i].second;
            if (!strName.empty())
            {
               m_arrAttachment.push_back(std::pair<std::wstring, int>(strName, nSize));
            }

        }
        return true;
    }


    /*{=====================================================*//**
    @brief		ÊÇ·ñÒÑ¶Á
    @return		bool£º			ÊÇ·ñÒÑ¶Á
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    bool CPMail::IsRead() const
    {
        return m_bRead;
    }

    //! get message-id
    std::wstring CPMail::GetMessageID() const
    {
        return m_strMessageID;
    }

    //! get thread-topic
    std::wstring CPMail::GetThreadTopic() const
    {
        return m_strThreadTopic;
    }

    //! get thread-index
    std::wstring CPMail::GetThreadIndex() const
    {
        return m_strThreadIndex;
    }

    //! get reply-to
    std::wstring CPMail::GetReplyTo() const
    {
        return m_strReplyTo;
    }

    //! get in-reply-to
    std::wstring CPMail::GetInReplyTo() const
    {
        return m_strInReplyTo;
    }


    /*{=====================================================*//**
    @brief		µÃµ½ÃèÊö
    @param		ÎÞ
    @return		std::wstring£º		ÃèÊö×Ö·û´®
    @author		ÌÀÎ°
    @date		2008.09.10
    @remark
    *///}========================================================
    std::wstring CPMail::GetDescription() const
    {
        return _T("");
    }


    /*{=====================================================*//**
    @brief		ÉèÖÃ·¢ËÍÊ±¼ä
    @param		[ÊäÈë]£º		time£º      ·¢ËÍÊ±¼ä
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetSendTime(const CPTime& time)
    {
        m_tSend = time;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ½ÓÊÕÊ±¼ä
    @param		[ÊäÈë]£º		time£º      ½ÓÊÕÊ±¼ä
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetReceiveTime(const CPTime& time)
    {
        m_tReceive = time;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ±£´æµ½±¾µØÊ±¼ä
    @param		[ÊäÈë]£º		time£º      ±£´æµ½±¾µØÊ±¼ä
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetSaveTime(const CPTime& time)
    {
        m_tSave = time;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃÒÑ¶Á±êÖ¾
    @param		[ÊäÈë]£º		bRead£º      ÊÇ·ñÒÑ¶Á
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetRead(bool bRead)
    {
        m_bRead = bRead;
    }

    //! set message-id
    void CPMail::SetMessageID(const std::wstring & str)
    {
        m_strMessageID = str;
    }

    //! set thread-topic
    void CPMail::SetThreadTopic(const std::wstring & str)
    {
        m_strThreadTopic = str;
    }

    //! set thread-index
    void CPMail::SetThreadIndex(const std::wstring & str)
    {
        m_strThreadIndex = str;
    }

    //! set reply-to
    void CPMail::SetReplyTo(const std::wstring & str)
    {
        m_strReplyTo = str;
    }

    //! set in-reply-to
    void CPMail::SetInReplyTo(const std::wstring & str)
    {
        m_strInReplyTo = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃÓÊ¼þÄ¿Â¼µÄ±ðÃû
    @param		[ÊäÈë]£º		str£º      ÓÊ¼þÄ¿Â¼µÄ±ðÃû
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetFolderAlias(const std::wstring& str)
    {
        m_strFolderAlias = str;
    }

    /*{=====================================================*//**
    @brief		µÃµ½ÓÊ¼þÄ¿Â¼µÄ±ðÃû
    @param		ÎÞ
    @return		std::wstring£º      ÓÊ¼þÄ¿Â¼µÄ±ðÃû
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    std::wstring CPMail::GetFolderAlias() const
    {
        return m_strFolderAlias;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃÕË»§ÃÜÂë
    @param		[ÊäÈë]£º		str£º      ÕË»§ÃÜÂë
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetAccountPassword(const std::wstring& str)
    {
        m_strAccountPassword = str;
    }

    /*{=====================================================*//**
    @brief		µÃµ½ÕË»§ÃÜÂë
    @param		ÎÞ
    @return		std::wstring£º      ÕË»§ÃÜÂë
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    std::wstring CPMail::GetAccountPassword() const
    {
        return m_strAccountPassword;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ·ÃÎÊÃÜÂë
    @param		[ÊäÈë]£º		str£º      ·ÃÎÊÃÜÂë
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetAccessPassword(const std::wstring& str)
    {
        m_strAccessPassword = str;
    }

    /*{=====================================================*//**
    @brief		µÃµ½·ÃÎÊÃÜÂë
    @param		ÎÞ
    @return		std::wstring£º      ·ÃÎÊÃÜÂë
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    std::wstring CPMail::GetAccessPassword() const
    {
        return m_strAccessPassword;
    }

    //! set Retrun-Path
    void CPMail::SetReturnPath(const std::wstring& str)
    {
        m_strReturnPath = str;
    }

    // get Return-Path
    std::wstring CPMail::GetReturnPath() const
    {
        return m_strReturnPath;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ·¢¼þÈË
    @param		[ÊäÈë]£º		str£º      ·¢¼þÈË
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetFrom(const std::wstring& str)
    {
        m_strFrom = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃÊÕ¼þÈË
    @param		[ÊäÈë]£º		str£º      ÊÕ¼þÈË
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetTo(const std::wstring& str)
    {
        m_strTo = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ°µËÍÐÅÏä
    @param		[ÊäÈë]£º		str£º      °µËÍÐÅÏä
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetBcc(const std::wstring& str)
    {
        m_strBcc = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ³­ËÍÐÅÏä
    @param		[ÊäÈë]£º		str£º      ³­ËÍÐÅÏä
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetCc(const std::wstring& str)
    {
        m_strCc = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ´¿ÎÄ±¾
    @param		[ÊäÈë]£º		str£º      ´¿ÎÄ±¾
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetText(const std::wstring& str)
    {
        m_strText = str;
    }

    /*{=====================================================*//**
    @brief		ÉèÖÃ±êÇ©ÀàÐÍ
    @param		[ÊäÈë]£º		nMark£º      ±êÇ©ÀàÐÍ
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetMark(byte nMark)
    {
        m_nMark = nMark;
    }

    /*{=====================================================*//**
    @brief		µÃµ½±êÇ©ÀàÐÍ
    @param		ÎÞ
    @return		byte£º      ±êÇ©ÀàÐÍ
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    byte CPMail::GetMark() const
    {
        return m_nMark;
    }

    /*{=====================================================*//**
    @brief		µÃµ½±êÇ©×Ö·û´®
    @param		ÎÞ
    @return		std::wstring£º      ±êÇ©×Ö·û´®
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    std::wstring CPMail::GetMarkString(byte nMark)
    {
        switch(nMark)
        {
        case 0:
            return L"无";
            break;
        case 1:
            return L"重要资料";
            break;
        case 2:
            return L"公司文件";
            break;
        case 3:
            return L"朋友来信";
            break;
        case 4:
            return L"趣闻轶事";
            break;
        case 5:
            return L"儿女情长";
            break;
        case 6:
            return L"业务往来";
            break;
        case 7:
            return L"技术交流";
            break;
        default:
            break;
        }

        return _T("");
    }

    /*{=====================================================*//**
    @brief		Ìí¼Ó¸½¼þÐÅÏ¢
    @param		[ÊäÈë]£º    attachment£º        ¸½¼þÐÅÏ¢
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    void CPMail::AddAttachment(const std::pair<std::wstring, uint>& attachment)
    {
        m_arrAttachment.push_back(attachment);
    }

    /*{=====================================================*//**
    @brief		Ìí¼Ó¸½¼þÐÅÏ¢
    @param		[ÊäÈë]£º            uint£º      Ë÷Òý
    @return		std::pair<std::wstring, uint>£º ¸½¼þÐÅÏ¢
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    std::pair<std::wstring, uint> CPMail::GetAttachment(uint index)
    {
        std::pair<std::wstring, uint> attachment;
        if (index < m_arrAttachment.size())
        {
            attachment = m_arrAttachment[index];
        }

        return attachment;
    }


#ifndef SI_MAC
    void CPMail::SetMailType(EPMailType eType)
    {
        m_eType = eType;
    }
#endif


    /*{=====================================================*//**
    @brief		ÉèÖÃÓÊ¼þ ID
    @param		[ÊäÈë]£º		nMailID£º      ÓÊ¼þ ID
    @return		void
    @author		ÌÀÎ°
    @date		2010.01.14
    @remark
    *///}========================================================
    void CPMail::SetMailID(uint64 nMailID)
    {
        m_nMailID = nMailID;
    }

    /*{=====================================================*//**
    @brief		µÃµ½ÓÊ¼þ ID
    @param		ÎÞ
    @return		uint64£º      ÓÊ¼þ ID
    @author		ÌÀÎ°
    @date		2010.01.15
    @remark
    *///}========================================================
    uint64 CPMail::GetMailID() const
    {
        return m_nMailID;
    }

    std::wstring CPMail::GetMailTypeName(EPRecordType eType)
    {
        std::wstring strTypeName;

        switch (eType)
        {
        case MAIL_RSS:
            strTypeName = L"RSS";
            break;

        case MAIL_MAIL:
            strTypeName = L"邮件";
            break;

        }
        return strTypeName;
    }
    EPRecordType CPMail::GetType()
    {
        return m_epRecordType;
    }

    void CPMail::SetType(EPRecordType type)
    {
        m_epRecordType = type;
    }
    void CPMail::SetSubject(const std::wstring& strSubject)
    {
        m_strSubject = strSubject;
    }
    std::wstring CPMail::GetSubject() const
    {
        return m_strSubject;
    }

    std::wstring CPMail::GetAttachment()
    {
        std::wstring attNames;
        for(size_t i = 0; i < m_arrAttachment.size(); i++)
        {
            if(i > 0)
            {
                attNames += L";";
            }

            attNames += m_arrAttachment[i].first;
        }
        return attNames;
    }
}


