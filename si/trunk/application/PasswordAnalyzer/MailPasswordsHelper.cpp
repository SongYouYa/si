#include "stdafx.h"
#include "MailPasswordsHelper.h"

#include <boost/xpressive/xpressive.hpp>
#include <Shlobj.h>

using namespace boost::xpressive;
using namespace boost::filesystem;
using namespace boost::algorithm;
typedef boost::filesystem::recursive_directory_iterator rd_iterator;

namespace password
{
	void CMailPasswordsHelper::GetAllMailPasswordsInformation(CPRecord* parentRecord)
    {
		MailPasswordInformation mail_information;
        GetOutlookPasswordsInformation(mail_information);
        GetWindowsLiveMailPasswordsInformation(mail_information);
        GetFoxmailPasswordsInformation(mail_information);

		for (auto it = mail_information.begin(); it != mail_information.end(); ++it)
		{
			CPRecord* record = parentRecord->addChild();
			record->at(L"MailType") = it->mail_type_;
			record->at(L"EmailAddress") = it->email_address_;
			record->at(L"Password") = it->password_;

			//如果和之前的相同，则删除
			std::vector<std::wstring> columnList;
			columnList.push_back(L"EmailAddress");
			columnList.push_back(L"Password");
			
			siutil::CPRecordHandler::removeLastSameChilren(parentRecord, columnList);
		}

		return;
    }
    void CMailPasswordsHelper::GetOutlookPasswordsInformation(MailPasswordInformation& mail_information)
    {
        Outlook2002To2010CommonPasswordsExceptExchangeServer(mail_information);
        Outlook2013CommonPasswordsExceptExchangeServer(mail_information);
    }
    void CMailPasswordsHelper::GetAllOEAccountFiles(const std::wstring& root_dir, std::vector<std::wstring>& oeaccount_files)
    {
        try
        {
            sregex_compiler rc;
            std::string file = "*.oeaccount";
            
            if(!rc[file].regex_id())
            {
                std::string str = replace_all_copy(replace_all_copy(file, ".", "\\."), "*", ".*");
                rc[file] = rc.compile(str);
            }
            
            rd_iterator end;
            
            for(rd_iterator pos(root_dir); pos != end; ++pos)
            {
                if(!is_directory(*pos) && regex_match(pos->path().filename().string(), rc[file]))
                {
                    oeaccount_files.push_back(pos->path().wstring());
                    //std::count<< " live mail account" << pos->path().string() << std::endl;
                }
            }
        }
        catch(...)
        {
            return;
        }
    }

    void CMailPasswordsHelper::GetWindowsLiveMailPasswordsInformation(MailPasswordInformation& mail_information)
    {
        std::wstring root_dir;
        wchar_t lpszPath[MAX_PATH];
        
        if(!SHGetSpecialFolderPath(NULL, lpszPath, CSIDL_LOCAL_APPDATA, false))
        {
            return;
        }
        
        root_dir = lpszPath;
        root_dir += L"\\Microsoft\\Windows Live Mail\\";
        std::vector<std::wstring> account_files;
        GetAllOEAccountFiles(root_dir, account_files);
        
        for (size_t i = 0; i < account_files.size(); ++i)
        {
            MailPasswordRecord mail_password_record;
            
            if (WindowsLiveMailAccountPasswords(mail_password_record, account_files.at(i)))
            {
                mail_information.push_back(mail_password_record);
            }
        }
    }
    void CMailPasswordsHelper::GetAllAccountFilesOfFoxmail(const std::wstring &root_dir, std::vector<std::wstring> &account_files)
    {
        try
        {
            std::wstring install_dir;
            install_dir = root_dir + L"/mail";
            directory_iterator end;
            
            for (directory_iterator pos(install_dir); pos != end; ++pos)
            {
                if (boost::filesystem::is_directory(*pos))
                {
                    boost::filesystem::path temp_path(*pos);
                    std::wstring account_file = temp_path.wstring() + L"/Account.stg";
                    
                    if (boost::filesystem::exists(*pos))
                    {
                        boost::replace_all(account_file, L"/", L"\\");
                        account_files.push_back(account_file);
                    }
                }
            }
        }
        catch (...)
        {
        }
    }
    void CMailPasswordsHelper::GetFoxmailPasswordsInformation(MailPasswordInformation& mail_information)
    {
        //get  the path of foxmail.exe by query register
        //HKEY_LOCAL_MACHINE\SOFTWARE\Clients\Mail\Foxmail\Protocols\mailto\DefaultIcon
        //HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Clients\Mail\Foxmail\Protocols\mailto\DefaultIcon
        //HKEY_CURRENT_USER\Software\Aerofox\Foxmail
        std::wstring register_6_5 = L"Software\\Aerofox\\Foxmail\\";
        std::wstring register_7 = L"Software\\Aerofox\\FoxmailPreview\\";
        std::vector<std::wstring> all_foxmail_versions;
        all_foxmail_versions.push_back(register_6_5);
        all_foxmail_versions.push_back(register_7);
        
        for (size_t i = 0; i < all_foxmail_versions.size(); i++)
        {
            std::wstring register_path = all_foxmail_versions.at(i);
            std::wstring foxmail_exe =L"";
            std::wstring foxmail_dir= L"";
            HKEY hKey;
            
            if(ERROR_SUCCESS ==RegOpenKeyW(HKEY_CURRENT_USER,register_path.c_str(),&hKey))
            {
                DWORD BufferLen;
                DWORD dwType;
                RegQueryValueExW(hKey,L"Executable",0,&dwType,NULL,&BufferLen);
                BYTE *Buffer = new BYTE[BufferLen];
                
                if(RegQueryValueExW(hKey, L"Executable",0,&dwType,Buffer,&BufferLen)==ERROR_SUCCESS)
                {
                    foxmail_exe = (wchar_t*) Buffer;
                }
                
                try
                {
                    foxmail_dir = boost::filesystem::path(foxmail_exe).parent_path().wstring();
                }
                catch(...)
                {
                }
            }
            else
            {
                continue;
            }
            
            if ( i == 0)
            {
                std::vector<std::wstring> accounts;
                GetAllAccountFilesOfFoxmail(foxmail_dir, accounts);
                
                for (size_t i = 0; i < accounts.size(); ++i)
                {
                    Foxmail6dot5AndLowerVersionsAccountPasswords(mail_information, accounts.at(i));
                }
            }
            else if (i ==1)
            {
                if (foxmail_dir.size() != 0)
                {
                    foxmail_dir = foxmail_dir + L"\\Data\\AccCfg\\Accounts.tdat";
                    boost::replace_all(foxmail_dir, L"/", L"\\");
                }
                
                if (boost::filesystem::exists(foxmail_dir))
                {
                    Foxmail7AndHigherVersionsAccountPasswords(mail_information, foxmail_dir);
                }
            }
        }
    }
}
