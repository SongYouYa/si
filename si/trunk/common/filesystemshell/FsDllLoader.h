#ifndef SI_FILESYSTEM_FSDLLLOADER_H
#define SI_FILESYSTEM_FSDLLLOADER_H
//加载文件系统相关的动态库，并从库中获取所需的函数
//本类只定义了一个接口，不同平台下有不同的实现。


//主要实现不同平台下动态库的加载方式
class CFsDllLoader
{
public:
    CFsDllLoader();
    virtual ~CFsDllLoader();
    void setDllDir(const std::wstring& dllDir);
    //加载动态库的接口函数
    bool Load();
    //释放所加载的库
    virtual void  Release();
    //取得函数入口地址
    virtual void* GetFunction(const char* strFuncName);
    class CImpl;
private:
    CImpl* GetImpl();
    CImpl* impl_;
};
//不同平台加载动态库的抽象接口
class CFsDllLoader::CImpl
{
public:
    virtual ~CImpl() {}
    CImpl() {}
    //创建具体实现类，在不同实现的CPP中实现
    static CImpl* Create();
    void setDllDir(const std::wstring& dllDir) {
        _dllDir = dllDir;
    }
    //加载动态库的接口函数
    virtual bool Load() = 0;
    //释放所加载的库
    virtual void  Release() = 0;
    //取得函数入口地址
    virtual void* GetFunction(const char* strFuncName) = 0;
protected:
    void* hModule_;
    std::wstring _dllDir;
};
#endif
