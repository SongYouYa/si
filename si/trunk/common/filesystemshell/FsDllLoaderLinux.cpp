#include "stdafx.h"
#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "FsDllLoader.h"

#include "sicommon/PUtils.h"
#include "sicommon/PCodeChanger.h"

class CFsDllLoaderLinux:public CFsDllLoader::CImpl
{
public:
    CFsDllLoaderLinux();
    virtual ~CFsDllLoaderLinux();
    //加载动态库的接口函数
    virtual bool Load();
    //释放所加载的库
    virtual void  Release();
    //取得函数入口地址
    virtual void* GetFunction(const char* strFuncName);
};
CFsDllLoaderLinux::CFsDllLoaderLinux()
{
}
CFsDllLoaderLinux::~CFsDllLoaderLinux()
{
    Release();
}
bool CFsDllLoaderLinux::Load()
{   
#ifdef __linux__
    const char* fsSoses[] =
    {
        "libSafeMedia.so",
        "libSafeLogical.so",
        "libNtfs.so",
        "libFat.so",
        "libExt.so",
        "libHFS.so",
        "libHFSPlus.so",
        "libApfs.so",
        "libCD_DVD.so",
        "libL01.so",
        "libYaffs2.so",
        "libUnknown.so",
        "libFileSystem.so"
    };
#elif OS_PAPPLE
    const char* fsSoses[] =
    {
        "libSafeLogical.dylib",
        "libNtfs.dylib",
        "libFat.dylib",
        "libExt.dylib",
        "libHFS.dylib",
        "libHFSPlus.dylib",
        "libCD_DVD.dylib",
        "libL01.dylib",
        "libYaffs2.dylib",
        "libUnknown.dylib",
        "libApfs.dylib",
        "libFileSystem.dylib"
    };
#endif
    
    std::string moduleDir = W2LS(_dllDir);
    if (!moduleDir.empty()) {
        moduleDir += "/";
    }
    for(size_t i = 0; i < sizeof(fsSoses) /sizeof(fsSoses[0]); i++)
    {
        std::string modulePath = moduleDir + fsSoses[i];
        hModule_ = dlopen(modulePath.c_str(),RTLD_NOW|RTLD_GLOBAL);
        if(hModule_ == 0) {
            char* err = dlerror();
            printf("load lib error:%s",err);
            return false;
        }
    }
    
    return hModule_ != NULL;
}
void CFsDllLoaderLinux::Release()
{
    if(hModule_)
        dlclose(hModule_);
}
void* CFsDllLoaderLinux::GetFunction(const char* strFuncName)
{
    return dlsym(hModule_,strFuncName);
}
CFsDllLoader::CImpl* CFsDllLoader::CImpl::Create()
{
    return new CFsDllLoaderLinux;
}
