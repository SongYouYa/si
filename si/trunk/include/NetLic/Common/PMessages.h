#pragma once

namespace auth
{
	enum MsgType
	{
		MSG_AUTH_APPLY = 1,
		MSG_AUTH_APPLY_RESULT,
		MSG_AUTH_QUIT_CLIENT,
		MSG_AUTH_KEEP_ALIVE,
	};

	enum AuthApplyStatus
	{
		APS_ERR_SUCCESS,
		APS_ERR_NO_MORE_LIC,
		APS_ERR_INTERNAL,
		APS_ERR_USE_TIME_EXPIRED,
		APS_ERR_SERVICE_TIME_EXPIRED
	};

	typedef int Product;

#pragma pack(push,1)
	struct CPMsgAuthHead
	{
		char			m_cFlag;
		unsigned short  m_nMsgLength;
		unsigned short	m_nMsgType;
	};


	struct CPMsgAuthApply : public CPMsgAuthHead
	{
	public:
		CPMsgAuthApply(){
			memset(this,0,sizeof(*this));
			m_cFlag = 'C';
			m_nMsgLength = sizeof(*this);
			m_nMsgType = MSG_AUTH_APPLY;
		}
		unsigned short  m_productID;		//要申请授权的产品ID
		char			m_enckey[8];		//告诉服务端用这个Key进行加密
		unsigned char	m_ValidateCode;		//消息校验码
	};

	struct CPMsgAuthApplyResult : public CPMsgAuthHead
	{
	public:
		CPMsgAuthApplyResult(){
			memset(this,0,sizeof(*this));
			m_cFlag = 'S';
			m_nMsgLength = sizeof(*this);
			m_nMsgType = MSG_AUTH_APPLY_RESULT;
		}
		unsigned short		m_resultCode;
		unsigned short		m_productID;
		DWORD				m_nSerial;
		FILETIME			m_prodEndSrvTime;
		FILETIME			m_prodEndUseTime;
		unsigned char		m_randValues[4];
		unsigned char		m_ValidateCode;
	};

	struct CPMsgAuthQuitClient : public CPMsgAuthHead
	{
	public:
		CPMsgAuthQuitClient(){
			memset(this,0,sizeof(*this));
			m_cFlag = 'S';
			m_nMsgLength = sizeof(*this);
			m_nMsgType = MSG_AUTH_QUIT_CLIENT;
		}
		unsigned short m_productID;
		char		   m_ip[16];//contains \0'
		unsigned char  rand[8];
		unsigned char  m_ValidateCode;
	};

	struct CPMsgAuthKeepAlive : public CPMsgAuthHead
	{
	public:
		CPMsgAuthKeepAlive(){
			memset(this,0,sizeof(*this));
			m_cFlag = 'C';// default from client
			m_nMsgLength = sizeof(*this);
			m_nMsgType = MSG_AUTH_KEEP_ALIVE;
		}
		unsigned short m_productID;
		char		   m_ip[16];
	};


#pragma pack(pop)

	struct CPServerAuthStatusEntry
	{
	public:
		CPServerAuthStatusEntry()
			: m_bAuthorized(false)
			, m_lastActiveTime(0)
			, m_nSerial(0)
			, m_productID(0)
		{
		}
		std::string		m_remoteIP;
		unsigned short	m_productID;
		bool			m_bAuthorized;
		time_t			m_lastActiveTime;
		unsigned int	m_nSerial;
		std::string		m_SessionKey;

	};

	

}

