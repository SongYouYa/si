#pragma once

#include <openssl/des.h>

class CDesAlgorithm{
public:
	CDesAlgorithm(int nBufferSize);
	virtual ~CDesAlgorithm();
	void  SetEncryptKey(char* password);
	char* Encrypt(char* pPlain,int size);
	char* Decrypt(char* pEncrypted,int size);
private:
	DES_cblock m_password;
	char*	   m_pResult;
	int		   m_nBufferSize;
	char	   m_key[8];
};
