#pragma once

#include "./PAuthClient.h"

namespace auth
{
	struct CPMsgAuthApplyResult;
	class CPAuthClientHandler
	{
	public:
		CPAuthClientHandler(CPAuthClient* client);
		virtual ~CPAuthClientHandler();

		void					SetProduct(Product prod);
		Product					GetProduct(){return m_product;}

		virtual AuthApplyStatus	OnMsgAuthApplyResult(CPMsgAuthApplyResult* result);

		CPMsgAuthApply*			GetMsgApplyAuth();
		char*					GetMsgAuthApplyEncryptKey();

		const FILETIME&			GetEndServiceDate(){return m_ftEndServiceTime;}
		const FILETIME&			GetEndUseDate(){return m_ftEndUseTime;}
		DWORD					GetDongleSerial(){return m_nSerial;}
	protected:

		CPAuthClient* m_client;

		CPMsgAuthApply*			m_MsgApplyAuth;//encrypted
		std::string				m_MsgAuthApplyResultDecryptKey;
		char					m_MsgEncryptKey[9];
		int						nlen;
		Product					m_product;
		FILETIME				m_ftEndServiceTime;
		FILETIME				m_ftEndUseTime;
		DWORD					m_nSerial;
		CRITICAL_SECTION        m_cs;
	protected:
		bool					IsValidMsg_AuthApplyResult(CPMsgAuthApplyResult* result);
	};
}
