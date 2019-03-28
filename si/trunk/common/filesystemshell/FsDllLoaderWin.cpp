#include "stdafx.h"
#include "FsDllLoader.h"
#include <wtypes.h>

class CImplWin: public CFsDllLoader::CImpl
{
public:
    CImplWin();
    virtual ~CImplWin();
    static CImpl* Create();
    virtual bool Load();
    virtual void* GetFunction( const char* strFuncName );
protected:
    virtual void Release();
    virtual void GetModulePath();
    std::wstring strModulePath_;
};
CImplWin::CImplWin()
{
}
CImplWin::~CImplWin()
{
    Release();
}
bool CImplWin::Load()
{
    if (strModulePath_.empty())
    {
        GetModulePath();
    }
    
    HMODULE hModule = ::LoadLibrary(strModulePath_.c_str());
    
    if (!hModule)
    {
        DWORD err = GetLastError();
        return false;
    }
    
    hModule_ = hModule;
    return true;
}
void CImplWin::Release()
{
    if (hModule_)
    {
        ::FreeLibrary((HMODULE)hModule_);
    }
}
void* CImplWin::GetFunction( const char* strFuncName )
{
    if (hModule_)
    {
        return (void*)GetProcAddress((HMODULE)hModule_,strFuncName);
    }
    
    return NULL;
}
void CImplWin::GetModulePath()
{
    WCHAR szFilePath[MAX_PATH+1] = {0};
    ::GetModuleFileNameW(NULL,(LPWSTR)szFilePath,MAX_PATH);
    std::wstring strModuleName(szFilePath);
    size_t pos = strModuleName.rfind(L"\\");
    strModulePath_ = strModuleName.substr(0,pos) + L"\\FileSystem.dll";
}
CFsDllLoader::CImpl* CFsDllLoader::CImpl::Create()
{
    return new CImplWin();
}
