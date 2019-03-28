#include "stdafx.h"
#include "DoDecrypt.h"

#include <urlhist.h>
#include <shlguid.h> // Needed for CLSID_CUrlHistory
#include <objbase.h>
#include <WinCred.h>

//#include <openssl/des.h>
//#include <openssl/md5.h>

#include "./tinyxml/tinyxml.h"

#pragma comment(lib, "Crypt32.lib")


#define URL_HISTORY_MAX 5000

namespace password
{
	//////////////////////////////////Get Outlook Passwords Information ////////////////////////////////////////
	/////Initial related field name in register that save outlook account information/////////////
	void GetRegisterFieldInformation(MailAccountFieldInformation& register_field_info)
	{
		MailAccountFieldRecord pop3_field_record;
		pop3_field_record.account_type_ = L"POP3";
		pop3_field_record.user_name_ = L"POP3 User";
		pop3_field_record.server_ = L"POP3 Server";
		pop3_field_record.port_ = L"POP3 Port";
		pop3_field_record.password_ = L"POP3 Password";
		pop3_field_record.address_ = L"Email";
		register_field_info.push_back(pop3_field_record);
		MailAccountFieldRecord smtp_field_record;
		smtp_field_record.account_type_ = L"SMTP";
		smtp_field_record.user_name_ = L"SMTP User";
		smtp_field_record.server_ = L"SMTP Server";
		smtp_field_record.port_ = L"SMTP Port";
		smtp_field_record.password_ = L"SMTP Password";
		smtp_field_record.address_ = L"Email";
		register_field_info.push_back(smtp_field_record);
		MailAccountFieldRecord imap_field_record;
		imap_field_record.account_type_ = L"IMAP";
		imap_field_record.user_name_ = L"IMAP User";
		imap_field_record.server_ = L"IMAP Server";
		imap_field_record.port_ = L"IMAP Port";
		imap_field_record.password_ = L"IMAP Password";
		imap_field_record.address_ = L"Email";
		register_field_info.push_back(imap_field_record);
		MailAccountFieldRecord http_field_record;
		http_field_record.account_type_ = L"HTTP";
		http_field_record.user_name_ = L"HTTP User";
		http_field_record.server_ = L"HTTP Server URL";
		http_field_record.port_ = L"N/A";
		http_field_record.password_ = L"HTTP Password";
		http_field_record.address_ = L"Email";
		register_field_info.push_back(http_field_record);
	}

    void Outlook2002To2010CommonPasswordsExceptExchangeServer(MailPasswordInformation& mail_passwords)
    {
        MailAccountFieldInformation register_field_info;
        GetRegisterFieldInformation(register_field_info);
        wchar_t *KeyStr = {L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows Messaging Subsystem\\Profiles\\"};
        HKEY hKey;
        
        if(ERROR_SUCCESS==RegOpenKeyW(HKEY_CURRENT_USER,KeyStr,&hKey))
        {
            for(int i=0;; i++)
            {
                WCHAR szBuffer[MAX_PATH] = {0};
                DWORD cName = _countof(szBuffer);
                
                if(RegEnumKeyExW(hKey,i,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS)
                {
                    std::wstring szKey1 = KeyStr;
                    szKey1 += szBuffer;
                    szKey1 += L"\\";
                    HKEY hKey1;
                    
                    if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey1.c_str(),&hKey1)!=ERROR_SUCCESS )
                    {
                        continue;
                    }
                    
                    WCHAR szBuffer[MAX_PATH] = {0};
                    DWORD cName = _countof(szBuffer);
                    
                    for ( int j=0; RegEnumKeyExW(hKey1,j,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++j, cName = _countof(szBuffer) )
                    {
                        std::wstring szKey2 = szKey1;
                        szKey2 += szBuffer;
                        szKey2 += L"\\";
                        HKEY hKey2;
                        
                        if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey2.c_str(),&hKey2)!=ERROR_SUCCESS )
                        {
                            continue;
                        }
                        
                        WCHAR szBuffer[MAX_PATH];
                        DWORD cName = _countof(szBuffer);
                        
                        for ( int k=0; RegEnumKeyExW(hKey2,k,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++k, cName = _countof(szBuffer) )
                        {
                            std::wstring szKey3 = szKey2;
                            szKey3 += szBuffer;
                            szKey3 += L"\\";
                            HKEY hKey3;
                            
                            if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey3.c_str(),&hKey3)!=ERROR_SUCCESS )
                            {
                                continue;
                            }
                            
                            DWORD BufferLen;
                            DWORD dwType;
                            
                            for (int srf = 0; srf < register_field_info.size(); ++srf)
                            {
                                std::wstring user_name = register_field_info.at(srf).user_name_;
                                std::wstring password  = register_field_info.at(srf).password_;
                                std::wstring email_address = register_field_info.at(srf).address_;
                                std::wstring server = register_field_info.at(srf).server_;
                                std::wstring port = register_field_info.at(srf).port_;
                                RegQueryValueExW(hKey3, user_name.c_str(),0,&dwType,NULL,&BufferLen);
                                wchar_t *Buffer = new wchar_t[BufferLen];
                                
                                if(RegQueryValueExW(hKey3,  user_name.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                {
                                    MailPasswordRecord mail_record;
                                    mail_record.username_ = (std::wstring)Buffer;
                                    delete Buffer;
                                    /*DWORD BufferLen;
                                    DWORD dwType;*/
                                    RegQueryValueExW(hKey3, server.c_str(),0,&dwType,NULL,&BufferLen);
                                    Buffer = new wchar_t[BufferLen];
                                    
                                    if(RegQueryValueExW(hKey3, server.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                    {
                                        mail_record.server_ = (std::wstring)Buffer;
                                        delete Buffer;
                                    }
                                    
                                    RegQueryValueExW(hKey3, email_address.c_str(),0,&dwType,NULL,&BufferLen);
                                    /*wchar_t **/Buffer = new wchar_t[BufferLen];
                                    
                                    if(RegQueryValueExW(hKey3, email_address.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                    {
                                        mail_record.email_address_ = (std::wstring)Buffer;
                                        delete Buffer;
                                    }
                                    
                                    RegQueryValueExW(hKey3, port.c_str(),0,&dwType,NULL,&BufferLen);
                                    /*wchar_t **/Buffer = new wchar_t[BufferLen];
                                    
                                    if(RegQueryValueExW(hKey3, port.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                    {
                                        mail_record.port_ = (std::wstring)Buffer;
                                        delete Buffer;
                                    }
                                    
                                    /*DWORD BufferLen;
                                    DWORD dwType;*/
                                    RegQueryValueExW(hKey3, password.c_str(),0,&dwType,NULL,&BufferLen);
                                    BYTE *BBuffer = new BYTE[BufferLen];
                                    
                                    if(RegQueryValueExW(hKey3,password.c_str(),0,&dwType,BBuffer,&BufferLen)==ERROR_SUCCESS)
                                    {
                                        DATA_BLOB DataIn;
                                        DATA_BLOB DataOut;
                                        DataIn.pbData = BBuffer + 1;
                                        DataIn.cbData = BufferLen - 1;
                                        
                                        //	std::cout<<DataIn.pbData<<std::endl;
                                        //CryptUnprotectData 确实在Wincrypt.h中，但是是定义在 Platform SDK中
                                        if(CryptUnprotectData(&DataIn,0,NULL,NULL,NULL,1,&DataOut))
                                        {
                                            mail_record.password_ = (wchar_t* ) DataOut.pbData;
                                            mail_record.mail_type_ = L"Outlook 2002-2010";
                                            mail_passwords.push_back(mail_record);
                                            LocalFree(DataOut.pbData);
                                        }
                                    }
                                }
                                else
                                {
                                    continue;;
                                }
                            }
                            
                            RegCloseKey(hKey3);
                        }
                        
                        RegCloseKey(hKey2);
                    }
                    
                    RegCloseKey(hKey1);
                }
                else
                {
                    break;
                }
            }
            
            RegCloseKey(hKey);
        }
    }
    void Outlook2013CommonPasswordsExceptExchangeServer(MailPasswordInformation& mail_passwords)
    {
        MailAccountFieldInformation register_field_info;
        GetRegisterFieldInformation(register_field_info);
        wchar_t *KeyStr = {L"Software\\Microsoft\\Office\\"};
        HKEY hKey;
        
        if(ERROR_SUCCESS==RegOpenKeyW(HKEY_CURRENT_USER,KeyStr,&hKey))
        {
            for(int i=0;; i++)
            {
                WCHAR szBuffer[MAX_PATH] = {0};
                DWORD cName = _countof(szBuffer);
                
                if(RegEnumKeyExW(hKey,i,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS)
                {
                    std::wstring szKeyNum = KeyStr;
                    szKeyNum += szBuffer;
                    szKeyNum += L"\\";
                    HKEY hKeyNum;
                    
                    if ( RegOpenKeyW(HKEY_CURRENT_USER,szKeyNum.c_str(),&hKeyNum)!=ERROR_SUCCESS )
                    {
                        continue;
                    }
                    
                    std::wstring strProfiles = szKeyNum + L"Outlook\\Profiles\\";
                    HKEY hKeyProfiles;
                    
                    if ( RegOpenKeyW(HKEY_CURRENT_USER,strProfiles.c_str(),&hKeyProfiles)!=ERROR_SUCCESS )
                    {
                        continue;
                    }
                    
                    for(int i=0;; i++)
                    {
                        WCHAR szBuffer[MAX_PATH] = {0};
                        DWORD cName = _countof(szBuffer);
                        
                        if(RegEnumKeyExW(hKeyProfiles,i,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS)
                        {
                            std::wstring szKey1 = strProfiles;
                            szKey1 += szBuffer;
                            szKey1 += L"\\";
                            HKEY hKey1;
                            
                            if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey1.c_str(),&hKey1)!=ERROR_SUCCESS )
                            {
                                continue;
                            }
                            
                            WCHAR szBuffer[MAX_PATH] = {0};
                            DWORD cName = _countof(szBuffer);
                            
                            for ( int j=0; RegEnumKeyExW(hKey1,j,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++j, cName = _countof(szBuffer) )
                            {
                                std::wstring szKey2 = szKey1;
                                szKey2 += szBuffer;
                                szKey2 += L"\\";
                                HKEY hKey2;
                                
                                if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey2.c_str(),&hKey2)!=ERROR_SUCCESS )
                                {
                                    continue;
                                }
                                
                                WCHAR szBuffer[MAX_PATH];
                                DWORD cName = _countof(szBuffer);
                                
                                for ( int k=0; RegEnumKeyExW(hKey2,k,szBuffer,&cName,NULL,NULL,NULL,NULL)==ERROR_SUCCESS; ++k, cName = _countof(szBuffer) )
                                {
                                    std::wstring szKey3 = szKey2;
                                    szKey3 += szBuffer;
                                    szKey3 += L"\\";
                                    HKEY hKey3;
                                    
                                    if ( RegOpenKeyW(HKEY_CURRENT_USER,szKey3.c_str(),&hKey3)!=ERROR_SUCCESS )
                                    {
                                        continue;
                                    }
                                    
                                    DWORD BufferLen;
                                    DWORD dwType;
                                    
                                    for (int srf = 0; srf < register_field_info.size(); ++srf)
                                    {
                                        std::wstring user_name = register_field_info.at(srf).user_name_;
                                        std::wstring password  = register_field_info.at(srf).password_;
                                        std::wstring email_address = register_field_info.at(srf).address_;
                                        std::wstring server = register_field_info.at(srf).server_;
                                        std::wstring port = register_field_info.at(srf).port_;
                                        RegQueryValueExW(hKey3, user_name.c_str(),0,&dwType,NULL,&BufferLen);
                                        wchar_t *Buffer = new wchar_t[BufferLen];
                                        
                                        if(RegQueryValueExW(hKey3,  user_name.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                        {
                                            MailPasswordRecord mail_record;
                                            mail_record.username_ = (std::wstring)Buffer;
                                            delete Buffer;
                                            /*DWORD BufferLen;
                                            DWORD dwType;*/
                                            RegQueryValueExW(hKey3, server.c_str(),0,&dwType,NULL,&BufferLen);
                                            Buffer = new wchar_t[BufferLen];
                                            
                                            if(RegQueryValueExW(hKey3, server.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                            {
                                                mail_record.server_ = (std::wstring)Buffer;
                                                delete Buffer;
                                            }
                                            
                                            RegQueryValueExW(hKey3, email_address.c_str(),0,&dwType,NULL,&BufferLen);
                                            /*wchar_t **/Buffer = new wchar_t[BufferLen];
                                            
                                            if(RegQueryValueExW(hKey3, email_address.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                            {
                                                mail_record.email_address_ = (std::wstring)Buffer;
                                                delete Buffer;
                                            }
                                            
                                            RegQueryValueExW(hKey3, port.c_str(),0,&dwType,NULL,&BufferLen);
                                            /*wchar_t **/Buffer = new wchar_t[BufferLen];
                                            
                                            if(RegQueryValueExW(hKey3, port.c_str(),0,&dwType,(LPBYTE)Buffer,&BufferLen)==ERROR_SUCCESS)
                                            {
                                                mail_record.port_ = (std::wstring)Buffer;
                                                delete Buffer;
                                            }
                                            
                                            /*DWORD BufferLen;
                                            DWORD dwType;*/
                                            RegQueryValueExW(hKey3, password.c_str(),0,&dwType,NULL,&BufferLen);
                                            BYTE *BBuffer = new BYTE[BufferLen];
                                            
                                            if(RegQueryValueExW(hKey3,password.c_str(),0,&dwType,BBuffer,&BufferLen)==ERROR_SUCCESS)
                                            {
                                                DATA_BLOB DataIn;
                                                DATA_BLOB DataOut;
                                                DataIn.pbData = BBuffer + 1;
                                                DataIn.cbData = BufferLen - 1;
                                                
                                                //	std::cout<<DataIn.pbData<<std::endl;
                                                //CryptUnprotectData 确实在Wincrypt.h中，但是是定义在 Platform SDK中
                                                if(CryptUnprotectData(&DataIn,0,NULL,NULL,NULL,1,&DataOut))
                                                {
                                                    mail_record.password_ = (wchar_t* ) DataOut.pbData;
                                                    mail_record.mail_type_ = L"Outlook 2013";
                                                    bool isExist = false;
                                                    MailPasswordInformation::iterator iterRecord = mail_passwords.begin();
                                                    
                                                    for (; iterRecord!=mail_passwords.end(); ++iterRecord)
                                                    {
                                                        if (mail_record.username_ == iterRecord->username_
                                                                && mail_record.password_ == iterRecord->password_)
                                                        {
                                                            isExist = true;
                                                            break;
                                                        }
                                                    }
                                                    
                                                    if (!isExist)
                                                    {
                                                        mail_passwords.push_back(mail_record);
                                                    }
                                                    
                                                    LocalFree(DataOut.pbData);
                                                }
                                            }
                                        }
                                        else
                                        {
                                            continue;;
                                        }
                                    }
                                    
                                    RegCloseKey(hKey3);
                                }
                                
                                RegCloseKey(hKey2);
                            }
                            
                            RegCloseKey(hKey1);
                        }
                        else
                        {
                            break;
                        }
                    }
                    
                    RegCloseKey(hKeyProfiles);
                    RegCloseKey(hKeyNum);
                }
                else
                {
                    break;
                }
            }
            
            RegCloseKey(hKey);
        }
    }

	//////////////////////////////////Get Windows Live Mail Accounts Password Information ////////////////////////////////////////
	boost::uint32_t GetWindowsLiveMailPort(const std::string port)
	{
		boost::uint32_t  total = 0;

		for (int i = port.size(); i >0; i--)
		{
			int base = 0;
			char tmp = port.at(i - 1);

			if (49 <= tmp && tmp <= 57)
			{
				base = tmp - 48;
			}
			else if (97 <= tmp && tmp <= 102)
			{
				base = tmp - 87;
			}
			else if (65 <= tmp && tmp <= 0)
			{
				base = tmp - 55;
			}

			if (base == 0)
			{
				continue;
			}

			for (int m = 0; m < port.size() - i; m++)
			{
				base = base * 16;
			}

			total = total + base;
		}

		return total;
	}

	void GetWindowsLiveMailAccountFieldInformation(MailAccountFieldInformation& mail_field)
	{
		MailAccountFieldRecord pop3_account;
		pop3_account.user_name_ = L"POP3_User_Name";
		pop3_account.password_ = L"POP3_Password2";
		pop3_account.address_ = L"SMTP_Email_Address";
		pop3_account.server_ = L"POP3_Server";
		pop3_account.port_ = L"POP3_Port";
		mail_field.push_back(pop3_account);
		MailAccountFieldRecord http_account;
		http_account.user_name_ = L"HTTPMail_User_Name";
		http_account.password_ = L"HTTPMail_Password2";
		http_account.address_ = L"SMTP_Email_Address";
		http_account.server_ = L"HTTPMail_Server";
		http_account.port_ = L"";
		mail_field.push_back(http_account);
		MailAccountFieldRecord imap_account;
		imap_account.user_name_ = L"IMAP_User_Name";
		imap_account.password_ = L"IMAP_Password2";
		imap_account.address_ = L"SMTP_Email_Address";
		imap_account.server_ = L"IMAP_Server";
		imap_account.port_ = L"IMAP_Port";
		mail_field.push_back(imap_account);
		MailAccountFieldRecord smtp_account;
		smtp_account.user_name_ = L"SMTP_User_Name";
		smtp_account.password_ = L"SMTP_Password2";
		smtp_account.address_ = L"SMTP_Email_Address";
		smtp_account.server_ = L"SMTP_Server";
		smtp_account.port_ = L"SMTP_Port";
		mail_field.push_back(smtp_account);
		MailAccountFieldRecord ldap_account;
		ldap_account.user_name_ = L"LDAP_User_Name";
		ldap_account.password_ = L"LDAP_Password2";
		ldap_account.address_ = L"";
		ldap_account.server_ = L"LDAP_Server";
		ldap_account.port_ = L"";
		mail_field.push_back(ldap_account);
	}

	bool WindowsLiveMailAccountPasswords(MailPasswordRecord& mail_account_record, const std::wstring& oeaccount_path)
	{
		MailAccountFieldInformation live_mail_account_field;
		GetWindowsLiveMailAccountFieldInformation(live_mail_account_field);
		HANDLE h = CreateFileW(oeaccount_path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

		if (h == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		DWORD size = GetFileSize(h, NULL);
		SetFilePointer(h, 2, NULL, FILE_BEGIN);
		wchar_t *buf = new wchar_t[size];
		memset(buf, 0, size);
		DWORD readsize;
		bool nRet = ReadFile(h, buf, size, &readsize, NULL);

		if (!nRet)
		{
			return false;
		}

		CloseHandle(h);
		int len = ::WideCharToMultiByte(CP_THREAD_ACP, 0, buf, -1, NULL, 0, NULL, NULL);

		if (len == -1)
		{
			return false;
		}

		char *bytedata = new char[len + 1];
		::WideCharToMultiByte(CP_THREAD_ACP, 0,
			buf,
			wcslen(buf) + 1, bytedata, len, NULL,
			NULL);
		std::map<std::string, std::string> m_data;
		TiXmlDocument *myDocument = new TiXmlDocument();
		myDocument->Parse(bytedata);
		TiXmlElement *RootElement = myDocument->RootElement();
		//cout << RootElement->Value() << endl;
		TiXmlElement *FirstNode = NULL;

		for (FirstNode = RootElement->FirstChildElement(); FirstNode != NULL; FirstNode = FirstNode->NextSiblingElement())
		{
			m_data[FirstNode->Value()] = FirstNode->FirstChild()->Value();
		}

		//std::string name;
		//std::string password;
		//std::string server;
		//name = "SMTP_Email_Address";//NNTP_Email_Address
		//server = "POP3_Server";//IMAP_Server NNTP_Server SMTP_Server (HTTPMail_Server)
		//password = "POP3_Password2";//IMAP_Password2 NNTP_Password2 SMTP_Password2
		std::string name;
		std::string password;
		std::string server;
		std::string port;
		std::string mail_address;

		for (int i = 0; i < live_mail_account_field.size(); ++i)
		{
			MailAccountFieldRecord account_field_record = live_mail_account_field.at(i);
			std::map<std::string, std::string>::iterator it = m_data.find(W2LS(account_field_record.user_name_));

			if (it != m_data.end())
			{
				name = it->second;
				it = m_data.find(W2LS(account_field_record.server_));

				if (it != m_data.end())
				{
					server = it->second;
				}

				it = m_data.find(W2LS(account_field_record.password_));

				if (it != m_data.end())
				{
					password = it->second;
				}

				if (account_field_record.port_.size() != 0)
				{
					it = m_data.find(W2LS(account_field_record.port_));

					if (it != m_data.end())
					{
						port = it->second;
					}
				}

				if (account_field_record.address_.size() != 0)
				{
					it = m_data.find(W2LS(account_field_record.address_));

					if (it != m_data.end())
					{
						mail_address = it->second;
					}
				}

				break;
			}
		}

		if (name.size() != 0 && password.size() != 0)
		{
			mail_account_record.username_ = LS2W(name);
			mail_account_record.mail_type_ = L"Windows Live Mail";
			mail_account_record.server_ = LS2W(server);
			boost::uint32_t temp_port = GetWindowsLiveMailPort(port);
			std::string  port_str = boost::lexical_cast<std::string>(temp_port);
			mail_account_record.port_ = LS2W(port_str);
			mail_account_record.email_address_ = LS2W(mail_address);
			wchar_t*KeyStr = { L"Software\\Microsoft\\Windows Live Mail\\" };
			HKEY hKey;

			if (ERROR_SUCCESS == RegOpenKey(HKEY_CURRENT_USER, KeyStr, &hKey))
			{
				DWORD BufferLen;
				DWORD dwType;
				RegQueryValueEx(hKey, L"Salt", 0, &dwType, NULL, &BufferLen);
				BYTE *Buffer = new BYTE[BufferLen];

				if (RegQueryValueEx(hKey, L"Salt", 0, &dwType, Buffer, &BufferLen) == ERROR_SUCCESS)
				{
					DATA_BLOB DataIn;
					DATA_BLOB DataOut;
					DATA_BLOB option;
					option.pbData = Buffer;
					option.cbData = BufferLen;
					BYTE *pw = new BYTE[password.size() / 2 + 1];

					for (int i = 0; i < password.size() / 2; ++i)
					{
						pw[i] = strtoul(password.substr(i * 2, 2).c_str(), NULL, 16);
					}

					DataIn.pbData = pw;
					DataIn.cbData = password.size() / 2;

					if (CryptUnprotectData(&DataIn, NULL, &option, NULL, NULL, 0, &DataOut))
					{
						char TmpBuf[1024];
						int len = ::WideCharToMultiByte(CP_THREAD_ACP, 0, (wchar_t*)DataOut.pbData, -1, NULL, 0, NULL, NULL);

						if (-1 != len)
						{
							::WideCharToMultiByte(CP_THREAD_ACP, 0,
								(wchar_t*)DataOut.pbData,
								wcslen((wchar_t*)DataOut.pbData) + 1, TmpBuf, len, NULL,
								NULL);
						}

						password = TmpBuf;
						//std::cout<<name<<":"<<server<<":"<<password<<std::endl;
						mail_account_record.password_ = (wchar_t*)DataOut.pbData;
						return true;
					}
				}
			}
		}

		return false;
	}
  
    //////////////////////////////////Get FoxMail Accounts Password Information ////////////////////////////////////////
    int myAtoi(char c)
    {
        if(c <= '9')return (c - '0');
        
        if(c <= 'Z')return (c - 'A' + 10);
        
        return (c - 'a' + 10);
    }
    ////find the first appearance of the string f in source data stream
    ////then return the pointer behind f
    int FindFirstStr(const char *source, int size, const char* f, int fsize)
    {
        for(int i = 0; i < (size - fsize + 1); ++i)
        {
            int j = 0;
            
            for(; j < fsize; ++j)
            {
                if(source[i + j] != f[j])
                {
                    break;
                }
            }
            
            if(j == fsize)
            {
                return i + fsize;
            }
        }
        
        return size;
    }
    void GetFoxmail6dot5AndLowerVersionFieldInfor(MailAccountFieldInformation& mail_accounts)
    {
        MailAccountFieldRecord pop3_record;
        pop3_record.account_type_ = L"POP3";
        pop3_record.user_name_ = L"POP3Account";
        pop3_record.password_ = L"POP3Password";
        pop3_record.server_ = L"POP3Host";
        pop3_record.port_ = L"POP3Port";
        pop3_record.address_ = L"MailAddress";
        mail_accounts.push_back(pop3_record);
        //MailAccountFieldRecord smtp_record;
    }
    void GetFoxmail7AndHigherVersionFieldInfor(MailAccountFieldInformation& mail_accounts)
    {
        MailAccountFieldRecord account_record;
        account_record.account_type_ = L"ALL";
        account_record.user_name_ = L"Account";
        account_record.password_ = L"Password";
        account_record.server_ = L"IncomingServer";
        account_record.port_ = L"IncomingPort";
        account_record.address_ = L"Email";
        mail_accounts.push_back(account_record);
    }
    char CharToHex( char ch )
    {
        char ref = 0x00;
        
        if ((ch >= '0' && ch <= '9'))
        {
            ref = ch - '0';
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            ref = ch - 'A' + 0x0A;
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            ref = ch - 'a' + 0x0A;
        }
        
        return ref;
    }
    size_t ToBuffer(const std::string& strSrc, byte* buf, size_t len)
    {
        size_t nCount = 0;
        
        for (size_t i=0; i<strSrc.size(); i+=2, nCount++)
        {
            if (!isxdigit((unsigned char)strSrc[i]) || !isxdigit((unsigned char)strSrc[i+1]) || nCount >= len)
            {
                break;
            }
            
            buf[nCount] = CharToHex(strSrc[i]);
            buf[nCount] <<= 4;
            buf[nCount] |= CharToHex(strSrc[i+1]);
        }
        
        return nCount;
    }
    std::string  Foxmail7PasswordDecrypter(std::string& EnPassword)
    {
        if (EnPassword.empty())
        {
            return false;
        }
        
        unsigned int EnPasswordLen = EnPassword.length();
        unsigned int i;
        unsigned char frist_xor_value;
        unsigned char xor_value;
        unsigned int tmp_int = 0;
        unsigned char tmp_char;
        unsigned char decrypto_key_str[] = {0x7E,0x46,0x40,0x37,0x25,0x6D,0x24,0x7E,0x00};
        const size_t MaxLen  = 0x1000;
        char *DePassword = new char[MaxLen];
        memset(DePassword,0,MaxLen);
        unsigned int DePwdLen = EnPasswordLen/2;
        
        if (1 == EnPasswordLen%2)
        {
            return false;
        }
        
        unsigned char * hex_pwd = new unsigned char[DePwdLen];
        ToBuffer(EnPassword,hex_pwd,DePwdLen);
        
        //这里开始进行解密运算
        for (i = 0; i< strlen((const char *)decrypto_key_str); i++)
        {
            tmp_int += decrypto_key_str[i];
        }
        
        frist_xor_value = tmp_int % 0xff;
        xor_value = frist_xor_value ^ hex_pwd[0];
        
        for(i=1; i<=DePwdLen-1; i++)
        {
            tmp_char = hex_pwd[i] ^ decrypto_key_str[(i-1)%strlen((const char *)decrypto_key_str)];
            
            if (tmp_char > xor_value)
            {
                tmp_char = tmp_char - xor_value;
            }
            else
            {
                tmp_char = tmp_char + (0xff - xor_value);
            }
            
            DePassword[i-1] = tmp_char;
            xor_value = hex_pwd[i];
        }
        
        std::string password(DePassword,DePwdLen-1);
        delete[] DePassword;
        delete[] hex_pwd;
        return password;
    }
    void Foxmail7AndHigherVersionsAccountPasswords(MailPasswordInformation& mail_passwords, const std::wstring& account_tdat_path)
    {
        int nPassLen, i;           //== 密码长度
        unsigned char AA[8]= {0x7E ,0x46 ,0x40 ,0x37 ,0x25, 0x6D ,0x24 ,0x7E};//{'~','F','@','7','%','m','$','~'};  foxmail7
        //unsigned char AA[8]={0x7E, 0x64, 0x72, 0x61, 0x47, 0x6F, 0x6E, 0x7E};   //~draGon~   foxmail6.5
        unsigned char BB[40];  //== 加密的二进制密码，如BB6DFB5DF061
        unsigned char CC[40];  //== CC[i+1] = CC[i] ^ BB[i], CC[0]=5A
        unsigned char DD[40];  //== DD[i] = BB[i+1] ^ AA[i]， AA[i]不够时循环使用
        unsigned char EE[40];  //== 解开的明文 EE[i]=DD[i] - CC[i] , 小于0时加0xFF
        //std::string confXmlPath = "Accounts2.tdat";
        //std::string confXmlPath = "Accounts2.tdat";
        HANDLE h = CreateFileW(account_tdat_path.c_str(),GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,0, NULL);
        
        if(h == INVALID_HANDLE_VALUE)
        {
            return ;
        }
        
        DWORD size = GetFileSize(h,NULL);
        char *buf = new char[size];
        char *pbuf = buf;
        memset(buf, 0, size);
        DWORD readsize;
        bool nRet = ReadFile(h, buf, size, &readsize, NULL);
        
        if(!nRet)
        {
            return ;
        }
        
        CloseHandle(h);
        int index = 0;
        std::vector<std::string> existsaccount;
        
        while(index < size)
        {
            char cEnPass[40];
            int tmpindex = index;
            pbuf = pbuf + index;
            size = size - index;
            std::string p = "Account";
            index = FindFirstStr(pbuf, size, p.c_str(), p.size());
            
            if(index >= size)
            {
                return;
            }
            
            unsigned int usrsize = pbuf[index + 4];
            char *user = new char[usrsize + 1];
            memcpy(user, pbuf + index + 8, usrsize);
            user[usrsize] = '\0';
            p = "Email";
            index = FindFirstStr(pbuf, size, p.c_str(), p.size());
            
            if(index >= size)
            {
                return;
            }
            
            usrsize = pbuf[index + 4];
            char *email= new char[usrsize + 1];
            memcpy(email, pbuf + index + 8, usrsize);
            email[usrsize] = '\0';
            p = "IncomingPort";
            index = FindFirstStr(pbuf, size, p.c_str(), p.size());
            
            if(index >= size)
            {
                return;
            }
            
            unsigned int port_low_byte = (unsigned char)pbuf[index + 4];
            unsigned int port_high_byte = (unsigned char)pbuf[index + 5];
            int port = port_high_byte * 16*16 + port_low_byte;
            bool isGo = true;
            
            for(int i = 0; i < existsaccount.size(); ++i)
            {
                if(existsaccount[i] == user)
                {
                    isGo = false;
                    p = "Password";
                    index = FindFirstStr(pbuf, size, p.c_str(), p.size());
                    break;
                }
            }
            
            if(!isGo)
            {
                continue;
            }
            
            //std::cout<<user<<std::endl;
            p = "IncomingServer";
            index = FindFirstStr(pbuf, size, p.c_str(), p.size());
            
            if(index >= size)
            {
                return;
            }
            
            usrsize = pbuf[index + 4];
            char *server = new char[usrsize + 1];
            memcpy(server, pbuf + index + 8, usrsize);
            server[usrsize] = '\0';
            //std::cout<<server<<std::endl;
            p = "Password";
            index = FindFirstStr(pbuf, size, p.c_str(), p.size());
            
            if(index >= size)
            {
                return;
            }
            
            usrsize = pbuf[index + 4];
            //std::cout<<m_cDePass<<std::endl;
            std::string user_name = user;
            std::string server_name = server;
            std::string email_address = email;
            std::string account_password="";
            
            if (usrsize != 0)
            {
                char *password = new char[usrsize + 1];
                memcpy(password, pbuf  + index + 8, usrsize);
                password[usrsize] = '\0';
                account_password = Foxmail7PasswordDecrypter(std::string(password));
                existsaccount.push_back(user);
                MailPasswordRecord sp;
                sp.mail_type_ = L"Foxmail 7-X";
                sp.username_ = LS2W(user_name);
				sp.server_ = LS2W(server_name);
				sp.password_ = LS2W(account_password);
				sp.email_address_ = LS2W(email_address);
                sp.port_ = boost::lexical_cast<std::wstring>(port);
                mail_passwords.push_back(sp);
                delete password;
            }
            
            delete user;
            delete email;
            delete server;
        }
    }
    void Foxmail6dot5AndLowerVersionsAccountPasswords(MailPasswordInformation& mail_passwords, const std::wstring& account_stg_path)
    {
        MailAccountFieldInformation mail_account_fields;
        GetFoxmail6dot5AndLowerVersionFieldInfor(mail_account_fields);
        int nPassLen, i;           //== 密码长度
        //unsigned char AA[8]= {0x7E ,0x46 ,0x40 ,0x37 ,0x25, 0x6D ,0x24 ,0x7E};//{'~','F','@','7','%','m','$','~'};  foxmail7
        unsigned char AA[8]= {0x7E, 0x64, 0x72, 0x61, 0x47, 0x6F, 0x6E, 0x7E};  //~draGon~   foxmail6.5
        unsigned char BB[40];  //== 加密的二进制密码，如BB6DFB5DF061
        unsigned char CC[40];  //== CC[i+1] = CC[i] ^ BB[i], CC[0]=5A
        unsigned char DD[40];  //== DD[i] = BB[i+1] ^ AA[i]， AA[i]不够时循环使用
        unsigned char EE[40];  //== 解开的明文 EE[i]=DD[i] - CC[i] , 小于0时加0xFF
        //std::string confXmlPath = "Accounts2.tdat";
        //std::string confXmlPath = "Accounts2.tdat";
        HANDLE h = CreateFileW(account_stg_path.c_str(),GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,0, NULL);
        
        if(h == INVALID_HANDLE_VALUE)
        {
            return ;
        }
        
        DWORD size = GetFileSize(h,NULL);
        char *buf = new char[size];
        char *pbuf = buf;
        memset(buf, 0, size);
        DWORD readsize;
        bool nRet = ReadFile(h, buf, size, &readsize, NULL);
        
        if(!nRet)
        {
            return ;
        }
        
        CloseHandle(h);
        int index = 0;
        std::vector<std::string> existsaccount;
        
        while(index < size)
        {
            char cEnPass[40];
            int tmpindex = index;
            pbuf = pbuf + index;
            size = size - index;
            
            //std::
            //index = FindFirstStr(pbuf, size, email_address.c_str(), email_address.size());
            //char *email_address_ptr = NULL;
            //if(index < size)
            //{
            //	pbuf =pbuf +index;
            //	size = size - index;
            //	end = FindFirstStr(pbuf, size, p.c_str(), p.size());
            //	end = end -p.size();
            //	usrsize = end-1;
            //	email_address_ptr = new char[usrsize + 1];
            //	memcpy(email_address_ptr, pbuf +1, usrsize);
            //	email_address_ptr[usrsize] = '\0';
            //}
            //else
            //{
            //	return;
            //}
            for (int i = 0 ; i < mail_account_fields.size(); ++i)
            {
                MailAccountFieldRecord mail_account_record = mail_account_fields.at(i);
                std::string user_name = W2LS(mail_account_record.user_name_);
				std::string password_str = W2LS(mail_account_record.password_);
				std::string server_name = W2LS(mail_account_record.server_);
				std::string email_address = W2LS(mail_account_record.address_);
				std::string port = W2LS(mail_account_record.port_);
                index = FindFirstStr(pbuf, size, email_address.c_str(), email_address.size());
                char *email_address_ptr = NULL;
                std::string p = "\r\n";
                unsigned int usrsize = 0;
                unsigned int end =0;
                
                if(index < size)
                {
                    pbuf =pbuf +index;
                    size = size - index;
                    end = FindFirstStr(pbuf, size, p.c_str(), p.size());
                    end = end -p.size();
                    usrsize = end-1;
                    email_address_ptr = new char[usrsize + 1];
                    memcpy(email_address_ptr, pbuf +1, usrsize);
                    email_address_ptr[usrsize] = '\0';
                }
                else
                {
                    continue;;
                }
                
                index = FindFirstStr(pbuf, size, user_name.c_str(), user_name.size());
                
                if (index >= size)
                {
                    continue;
                }
                
                pbuf =pbuf +index;
                size = size - index;
                end = FindFirstStr(pbuf, size, p.c_str(), p.size());
                end = end -p.size();
                usrsize = end-1;
                char *user = new char[usrsize + 1];
                memcpy(user, pbuf +1, usrsize);
                user[usrsize] = '\0';
                index = FindFirstStr(pbuf, size, server_name.c_str(), server_name.size());
                
                if(index >= size)
                {
                    return;
                }
                
                pbuf =pbuf +index;
                size = size - index;
                end = FindFirstStr(pbuf, size, p.c_str(), p.size());
                end = end -p.size();
                usrsize = end-1;
                char *server = new char[usrsize + 1];
                memcpy(server, pbuf +1, usrsize);
                server[usrsize] = '\0';
                //p = "POP3Password";
                index = FindFirstStr(pbuf, size, password_str.c_str(), password_str.size());
                
                if(index >= size)
                {
                    return;
                }
                
                p="\r\n";
                pbuf =pbuf +index;
                size = size - index;
                end = FindFirstStr(pbuf, size, p.c_str(), p.size());
                end = end -p.size();
                //unsigned int usrsize = end-index;
                usrsize = end-1;
                //usrsize = pbuf[index + 4];
                char *password = new char[usrsize + 1];
                memcpy(password, pbuf+1, usrsize);
                password[usrsize] = '\0';
                //std::cout<<password<<std::endl;
                //	password[0] = 'F';
                //	password[1] = 'D';
                index = FindFirstStr(pbuf, size, port.c_str(), port.size());
                char *port_ptr = NULL;
                
                if(index < size)
                {
                    pbuf =pbuf +index;
                    size = size - index;
                    end = FindFirstStr(pbuf, size, p.c_str(), p.size());
                    end = end -p.size();
                    usrsize = end-1;
                    port_ptr = new char[usrsize + 1];
                    memcpy(port_ptr, pbuf +1, usrsize);
                    port_ptr[usrsize] = '\0';
                }
                
                std::string m_cEnPass = password;
                std::string m_cDePass = "";
                nPassLen = m_cEnPass.size();
                
                if(nPassLen %2 != 0)
                {
                    return;
                }
                
                nPassLen = nPassLen/2;
                strcpy(cEnPass, m_cEnPass.c_str());
                unsigned char b1, b2;
                BB[0]=0;
                
                for(i=1; i<=nPassLen; i++)
                {
                    b1 = myAtoi(cEnPass[2*i-2]);
                    b2 = myAtoi(cEnPass[2*i-1]);
                    BB[i]= (b1<<4) + b2;
                }
                
                CC[0]=0x5A;
                CC[1]=CC[0]^BB[1];
                
                for(i=2; i<=nPassLen; i++)
                {
                    CC[i]=BB[i];
                }
                
                DD[0]=0;
                
                for(i=1; i<nPassLen; i++)
                {
                    DD[i]= BB[i+1]^AA[(i-1)%8];
                }
                
                EE[0]=0;
                
                for(i=1; i<nPassLen; i++)
                {
                    EE[i]=DD[i]>CC[i]? (DD[i]-CC[i]) : (0xFF+DD[i]-CC[i]);
                }
                
                memset(cEnPass,1,40);
                
                for(i=0; i<nPassLen-1; i++) cEnPass[i]=EE[i+1];
                
                cEnPass[nPassLen-1]='\0';
                //cEnPass[0] = '*';
                m_cDePass = cEnPass;
                //std::cout<<m_cDePass<<std::endl;
                //std::string user_name = user;
                //std::string server_name = server;
                //std::string account_password = m_cDePass;
                MailPasswordRecord sp;
                sp.mail_type_ = L"Foxmail X-6.5";
                sp.username_ = LS2W(user);
				sp.server_ = LS2W(server);
				sp.password_ = LS2W(m_cDePass);
                delete user;
                delete server;
                delete password;
                
                if (email_address_ptr)
                {
					sp.email_address_ = LS2W(email_address_ptr);
                    delete email_address_ptr;
                }
                
                if (port_ptr)
                {
					sp.port_ = LS2W(port_ptr);
                    //sp.port_ = boost::lexical_cast<boost::uint32_t>(port_ptr);
                    delete port_ptr;
                }
                
                //sp.name = user;
                //sp.url = server;
                //sp.password = m_cDePass;
                //c.push_back(sp);
                mail_passwords.push_back(sp);
            }
            
            return;
        }
    }
}
