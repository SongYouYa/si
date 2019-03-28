#include "stdafx.h"
#include "Win32WMIHelper.h"
#define _WIN32_DCOM
#include <comdef.h>
#include <WbemIdl.h>
#include <boost/thread/mutex.hpp>

static boost::mutex s_instance_mutex;

namespace app
{
    class Win32WMIHelper::Win32WMIHelperImpl
    {
    public:
        Win32WMIHelperImpl();
        ~Win32WMIHelperImpl();
        bool IsInitialized();
        IEnumWbemClassObject* ExecuteQuery(const std::string& sql);
    private:
        IWbemLocator* locator_;
        IWbemServices* service_;

    public:
        static void ParseUserAcccountsInformaion(Win32UserAccount & user_account, IWbemClassObject * object);
        static void ParseAccountInformation(Win32LogonAccount& account, IWbemClassObject * object);
        static uint32 GetInt32(IWbemClassObject* object, const std::wstring& property_name, uint32 default = 0)
        {
            assert(object != NULL);
            VARIANT prop;
            HRESULT hr = object->Get(property_name.c_str(), 0, &prop, 0, 0);
            
            if (SUCCEEDED(hr) && prop.vt == VT_I4)
            {
                return prop.intVal;
            }
            else
            {
                return default;
            }
        }
        static uint32 GetUInt32(IWbemClassObject* object, const std::wstring& property_name, uint32 default = 0)
        {
            assert(object != NULL);
            VARIANT prop;
            HRESULT hr = object->Get(property_name.c_str(), 0, &prop, 0, 0);
            
            if (SUCCEEDED(hr) && prop.vt == VT_UI4)
            {
                return prop.uintVal;
            }
            else
            {
                return default;
            }
        }
        static uint64_t GetUInt64(IWbemClassObject* object, const std::wstring& property_name, uint64_t default = 0)
        {
            assert(object != NULL);
            VARIANT prop;
            HRESULT hr = object->Get(property_name.c_str(), 0, &prop, 0, 0);
            
            if (SUCCEEDED(hr) && prop.vt == VT_UI8)
            {
                return prop.ullVal;
            }
            else
            {
                return default;
            }
        }
        static BSTR GetBSTR(IWbemClassObject* object, const std::wstring& property_name, const BSTR default = L"")
        {
            assert(object != NULL);
            VARIANT prop;
            HRESULT hr = object->Get(property_name.c_str(), 0, &prop, 0, 0);
            
            if (SUCCEEDED(hr) && prop.vt == VT_BSTR)
            {
                return prop.bstrVal;
            }
            else
            {
                return default;
            }
        }
        static bool GetBool(IWbemClassObject* object, const std::wstring& property_name, bool default = false)
        {
            assert(object != NULL);
            VARIANT prop;
            HRESULT hr = object->Get(property_name.c_str(), 0, &prop, 0, 0);
            
            if (SUCCEEDED(hr) &&prop.vt == VT_BOOL)
            {
                return prop.boolVal ? true : false;
            }
            else
            {
                return default;
            }
        }
    };
    Win32WMIHelper::Win32WMIHelperImpl::Win32WMIHelperImpl() : locator_(NULL), service_(NULL)
    {
        //Initialize COM.
        HRESULT hr = ::CoInitializeEx(0, COINIT_MULTITHREADED);
        
        if (FAILED(hr))
        {
            //return;
        }
        
        //Set general COM security levels
        hr = ::CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT,
                                    RPC_C_IMP_LEVEL_IMPERSONATE,
                                    NULL, EOAC_NONE, NULL);
                                    
        if (FAILED(hr) && hr != RPC_E_TOO_LATE)
        {
            CoUninitialize();
            return;
        }
        
        //Obtain the initial locator to WMI
        hr = ::CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                                IID_IWbemLocator, (LPVOID*)&locator_);
                                
        if (FAILED(hr))
        {
            ::CoUninitialize();
            return;
        }
        
        //Connect to WMI through the IWemLocator::ConnectServer method
        hr = locator_->ConnectServer(_bstr_t(L"ROOT\\CIMV2"),
                                     NULL, NULL, 0, NULL, 0, 0, &service_);
                                     
        if (FAILED(hr))
        {
            locator_->Release();
            locator_ = NULL;
            ::CoUninitialize();
            return;
        }
        
        //Set security levels on the proxy
        hr = ::CoSetProxyBlanket(service_, RPC_C_AUTHN_WINNT,
                                 RPC_C_AUTHN_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
                                 RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
                                 
        if (FAILED(hr))
        {
            service_->Release();
            service_ = NULL;
            locator_->Release();
            locator_ = NULL;
            ::CoUninitialize();
        }
    }
    Win32WMIHelper::Win32WMIHelperImpl::~Win32WMIHelperImpl()
    {
        if (locator_)
        {
            locator_->Release();
            locator_ = NULL;
        }
        
        if (service_)
        {
            service_->Release();
            service_ = NULL;
            ::CoUninitialize();
        }
    }
    bool Win32WMIHelper::Win32WMIHelperImpl::IsInitialized()
    {
        return service_ ? true : false;
    }
    IEnumWbemClassObject* Win32WMIHelper::Win32WMIHelperImpl::ExecuteQuery(const std::string& sql)
    {
        IEnumWbemClassObject* enumerator = NULL;
        
        if (service_)
        {
            HRESULT hr = service_->ExecQuery(bstr_t("WQL"), bstr_t(sql.c_str()),
                                             WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                             NULL, &enumerator);
        }
        
        return enumerator;
    }
   
    void Win32WMIHelper::Win32WMIHelperImpl::ParseAccountInformation(Win32LogonAccount& account, IWbemClassObject * object)
    {
        assert(object != NULL);
        account.caption_ = Win32WMIHelperImpl::GetBSTR(object, L"Caption");
        account.description_ = Win32WMIHelperImpl::GetBSTR(object, L"Description");
        account.domain_ = Win32WMIHelperImpl::GetBSTR(object, L"Domain");
        account.local_account_ =Win32WMIHelperImpl::GetBool(object, L"LocalAccount");
        account.name_ = Win32WMIHelperImpl::GetBSTR(object, L"Name");
        account.sid_ =Win32WMIHelperImpl::GetBSTR(object, L"SID");
        account.sid_type_=Win32WMIHelperImpl::GetUInt32(object, L"SIDType");
        account.status_ = Win32WMIHelperImpl::GetBSTR(object, L"Status");
    }
    void Win32WMIHelper::Win32WMIHelperImpl::ParseUserAcccountsInformaion(Win32UserAccount & user_account, IWbemClassObject * object)
    {
        assert(object != NULL);
        user_account.account_type_ = Win32WMIHelperImpl::GetUInt32(object, L"AccountType");
        user_account.caption_ = Win32WMIHelperImpl::GetBSTR(object, L"Caption");
        user_account.description_ = Win32WMIHelperImpl::GetBSTR(object, L"Description");
        user_account.disabled_ = Win32WMIHelperImpl::GetBool(object, L"Disabled");
        user_account.domain_ = Win32WMIHelperImpl::GetBSTR(object, L"Domain");
        user_account.full_name_ = Win32WMIHelperImpl::GetBSTR(object, L"FullName");
        user_account.local_account_ =Win32WMIHelperImpl::GetBool(object, L"LocalAccount");
        user_account.lockout_=Win32WMIHelperImpl::GetBool(object, L"Lockout");
        user_account.name_ = Win32WMIHelperImpl::GetBSTR(object, L"Name");
        user_account.password_changeable_=Win32WMIHelperImpl::GetBool(object, L"PasswordChangeable");
        user_account.password_expires_ =Win32WMIHelperImpl::GetBool(object, L"PasswordExpires");
        user_account.password_required_ =Win32WMIHelperImpl::GetBool(object, L"PasswordRequired");
        user_account.sid_ =Win32WMIHelperImpl::GetBSTR(object, L"SID");
        user_account.sid_type_=Win32WMIHelperImpl::GetUInt32(object, L"SIDType");
        user_account.status_ = Win32WMIHelperImpl::GetBSTR(object, L"Status");
    }
    
	Win32WMIHelper::Win32WMIHelper() : implement_(new Win32WMIHelperImpl())
	{
	}

	Win32WMIHelper::~Win32WMIHelper()
	{
		//                 if (implement_)
		//                 {
		//                     delete implement_;
		//                 }
		//                 implement_ = NULL;
	}
    
    bool Win32WMIHelper::GetUserAccountsInformation(std::vector<Win32UserAccount> & user_accounts)
    {
        user_accounts.clear();
        
        if (!implement_->IsInitialized())
        {
            return false;
        }
        
        IEnumWbemClassObject* enumerator = implement_->ExecuteQuery("select * from Win32_UserAccount");
        
        if(enumerator == NULL)
        {
            implement_ = new Win32WMIHelperImpl;
            
            if(implement_)
            {
                enumerator = implement_->ExecuteQuery("select * from Win32_UserAccount");
            }
        }
        
        IWbemClassObject* object = NULL;
        ULONG return_value = 0;
        
        while (enumerator)
        {
            HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &return_value);
            
            if (0 == return_value)
            {
                break;
            }
            
            Win32UserAccount user_account;
            Win32WMIHelperImpl::ParseUserAcccountsInformaion(user_account, object);
            user_accounts.push_back(user_account);
        }
        
        if (enumerator)
        {
            enumerator->Release();
        }
        
        if (object)
        {
            object->Release();
        }
        
        return true;
    }

	bool Win32WMIHelper::GetLogonTime(std::wstring logon_id, std::wstring& logon_time)
	{
		std::string sql = "select * from Win32_LogonSession where LogonId ='";
		sql.append(CPCodeChanger::UnicodeToUTF8(logon_id.c_str()).c_str());
		sql.append("'");
		IEnumWbemClassObject* enumerator = implement_->ExecuteQuery(sql);

		if (enumerator == NULL)
		{
			implement_ = new Win32WMIHelperImpl;

			if (implement_)
			{
				enumerator = implement_->ExecuteQuery(sql);
			}
		}

		IWbemClassObject* object = NULL;
		ULONG return_value = 0;

		while (enumerator)
		{
			HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &return_value);

			if (0 == return_value)
			{
				break;
			}

			logon_time = Win32WMIHelperImpl::GetBSTR(object, L"StartTime");
		}

		if (enumerator)
		{
			enumerator->Release();
		}

		if (object)
		{
			object->Release();
		}

		return true;
	}
    
    time_t  Win32WMIHelper::ParseDateTimeToTimet(const std::wstring& date_time)
    {
		std::string str_yy = CPCodeChanger::UnicodeToGB(date_time.substr(0, 4));
		std::string str_mm = CPCodeChanger::UnicodeToGB(date_time.substr(4, 2).c_str());
		std::string str_dd = CPCodeChanger::UnicodeToGB(date_time.substr(6, 2).c_str());
		std::string str_hh = CPCodeChanger::UnicodeToGB(date_time.substr(8, 2).c_str());
		std::string str_min = CPCodeChanger::UnicodeToGB(date_time.substr(10, 2).c_str());
		std::string str_ss = CPCodeChanger::UnicodeToGB(date_time.substr(12, 2).c_str());
		std::string str_zone = CPCodeChanger::UnicodeToGB(date_time.substr(22, 3).c_str());
		SYSTEMTIME st;
        st.wYear = boost::lexical_cast<short>(str_yy);
        st.wMonth = boost::lexical_cast<uint16_t>(str_mm);
        st.wDay = boost::lexical_cast<uint16_t>(str_dd);
        st.wHour = boost::lexical_cast<uint16_t>(str_hh);
        st.wMinute = boost::lexical_cast<uint16_t>(str_min);
        st.wSecond = boost::lexical_cast<uint16_t>(str_ss);
        st.wMilliseconds = 0;
        uint16_t zone = boost::lexical_cast<uint16_t>(str_zone);
		//SYSTEMTIME st;
        time_t tmt = TimeConverter::SystemTimeToTimet(st);
		return tmt - zone*60;;
    }
	//获取系统登入信息
    bool Win32WMIHelper::GetLogonUserAccountsInformation(std::vector<Win32LogonAccount>& login_user_accounts)
    {
        login_user_accounts.clear();
        
        if (!implement_->IsInitialized())
        {
            return false;
        }
        
        IEnumWbemClassObject* enumerator = implement_->ExecuteQuery("select * from Win32_LogonSession");
        
        if(enumerator == NULL)
        {
            implement_ = new Win32WMIHelperImpl;
            
            if(implement_)
            {
                enumerator = implement_->ExecuteQuery("select * from Win32_LogonSession");
            }
        }
        
        IWbemClassObject* object = NULL;
        ULONG return_value = 0;
        
        while (enumerator)
        {
            HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &return_value);
            
            if (0 == return_value)
            {
                break;
            }
            
            std::wstring  logon_id = Win32WMIHelperImpl::GetBSTR(object, L"LogonID");
            std::string query = "ASSOCIATORS OF {Win32_LogonSession.LogonID='";
            query.append(CPCodeChanger::UnicodeToUTF8(logon_id.c_str()).c_str());
            query.append("'} WHERE AssocClass = Win32_LoggedOnUser");
            IEnumWbemClassObject* enumerator = implement_->ExecuteQuery(query);
            IWbemClassObject* object = NULL;
            
            while (enumerator)
            {
                HRESULT hr = enumerator->Next(WBEM_INFINITE, 1, &object, &return_value);
                
                if (0 == return_value)
                {
                    break;
                }
                
                Win32LogonAccount  user_account;
                Win32WMIHelperImpl::ParseAccountInformation(user_account, object);
                std::wstring start_time = L"";
                GetLogonTime(logon_id, start_time);
                user_account.start_time_ = ParseDateTimeToTimet(start_time);
				//摄取信息，到login_user_accounts容器
                login_user_accounts.push_back(user_account);
            }
            
            if (enumerator)
            {
                enumerator->Release();
                enumerator = NULL;
            }
            
            if (object != NULL)
            {
                object->Release();
                object = NULL;
            }
        }
        
        if (enumerator)
        {
            enumerator->Release();
        }
        
        if (object)
        {
            object->Release();
        }
        
        return true;
    }

    Win32WMIHelper& Win32WMIHelper::GetInstance()
    {
		boost::mutex::scoped_lock lock(s_instance_mutex);
        static Win32WMIHelper instance;
        return instance;
    }
}
