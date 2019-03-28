/*{=====================================================*//**
@file       PMail.h
@copyright  2009 Pansafe Technology, Inc.
@brief      ���ʼ�������
@author     ��ΰ
@date       2010.01.13
@remark     
*///}========================================================

#ifndef SA_INCLUDE_APPLICATION_MAIL_PMAIL_H
#define SA_INCLUDE_APPLICATION_MAIL_PMAIL_H

#ifndef SI_OFFLINE
#include "PMailStruct.h"
#include "base/PTime.h"
#endif

class CMimeMessage;
namespace macbasic
{
    class CPMail
    {
    public:
        CPMail(void);
        virtual ~CPMail(void);

        //! ����ʱ��
        void SetSendTime(const CPTime& time);
        CPTime GetSentTime() const;

        //! ���浽����ʱ��
        void SetSaveTime(const CPTime& time);
        CPTime GetSaveTime() const;

        //! ����ʱ��
        void SetReceiveTime(const CPTime& time);
        CPTime GetReceiveTime() const;

        //! �Ѷ�
        void SetRead(bool bRead);
        bool IsRead() const; 

        //! Message-ID 
        void SetMessageID(const std::wstring & str);
        std::wstring GetMessageID() const;

        //! Thread-Topic 
        void SetThreadTopic(const std::wstring & str);
        std::wstring GetThreadTopic() const;

        //! Thread-Index 
        void SetThreadIndex(const std::wstring & str);
        std::wstring GetThreadIndex() const;

        //! Reply-To 
        void SetReplyTo(const std::wstring & str);
        std::wstring GetReplyTo() const;

        //! In-Reply-To 
        void SetInReplyTo(const std::wstring & str);
        std::wstring GetInReplyTo() const;

        //! �ʼ�Ŀ¼����
        void SetFolderAlias(const std::wstring& str);
        std::wstring GetFolderAlias() const;

        //! �˻�����
        void SetAccountPassword(const std::wstring& str);
        std::wstring GetAccountPassword() const;

        //! ��������
        void SetAccessPassword(const std::wstring& str);
        std::wstring GetAccessPassword() const;

        //! Return-Path 
        void SetReturnPath(const std::wstring& str);
        std::wstring GetReturnPath() const;

        //! ������
        void SetFrom(const std::wstring& str);
        std::wstring GetFrom() const;

        //! �ռ���
        void SetTo(const std::wstring& str);
        std::wstring GetTo() const;

        //! ����
        void SetBcc(const std::wstring& str);
        std::wstring GetBcc() const;

        //! ����
        void SetCc(const std::wstring& str);
        std::wstring GetCc() const;

        //! �ı�
        void SetText(const std::wstring& str);
        std::wstring GetText() const;
        static std::wstring HtmlToText(const std::string& strHtml);

        //! ��ǩ
        void SetMark(byte nMark);
        byte GetMark() const;
        static std::wstring GetMarkString(byte nMark);

        void SetSubject(const std::wstring& strSubject);
        std::wstring GetSubject() const;

        //! ����
        void SetMailType(EPMailType eType);

        //! ����
        virtual std::wstring GetDescription() const;

        //! ����
        void AddAttachment(const std::pair<std::wstring, uint>& attachment);
        std::pair<std::wstring, uint> GetAttachment(uint index);
        uint GetAttachmentCount() const;
        std::wstring GetAttachment();

        //! �ʼ�ID
        void SetMailID(uint64 nMailID);
        uint64 GetMailID() const;
        

        //! �����ʼ�
        bool Load(const char* buf, int nSize);
        
        EPRecordType GetType();
        void SetType(EPRecordType type);
        
    private:

        std::wstring GetMailTypeName(EPRecordType eType);        



        std::wstring		                        m_strFrom;				//! ������
        std::wstring		                        m_strTo;				//! �ռ���
        std::wstring		                        m_strCc;				//! ������
        std::wstring		                        m_strBcc;				//! ������
        std::wstring		                        m_strText;				//! �ı�
        CPTime                                      m_tSend;                //! �ʼ�����ʱ��
        CPTime			                            m_tReceive;             //! ����������ʱ��
        CPTime		    	                        m_tSave;                //! �ʼ������ڱ���ʱ��
        bool                                        m_bRead;                //! ��ʶ�ʼ��Ƿ��Ѷ�(�Ѷ�Ϊ0��δ��Ϊ1)
        std::wstring                                m_strMessageID;         //! Message-ID 
        std::wstring                                m_strThreadTopic;       //! Thread-Topic 
        std::wstring                                m_strThreadIndex;       //! Thread-Index 
        std::wstring                                m_strReplyTo;           //! Reply-To 
        std::wstring                                m_strInReplyTo;         //! In-Reply-To 
        byte                                        m_nMark;                //! ��ǩ��Ŀǰֻ�� Foxmail ������
        std::wstring                                m_strAccountPassword;   //! �˻����룬Ŀǰֻ��
        std::wstring                                m_strAccessPassword;    //! ��������
        std::wstring                                m_strReturnPath;        //! Return-Path 
        std::wstring                                m_strFolderAlias;       //! �ʼ��еı�ʶ

        EPMailType                                  m_eType;                //! �ʼ�����
        EPRecordType                                m_epRecordType;
        uint64                                      m_nMailID;              //! �ʼ�ID
        std::vector< std::pair<std::wstring, int> > m_arrAttachment;        //! ������Ϣ
        std::wstring                                m_strSubject;
    };
};
#endif
