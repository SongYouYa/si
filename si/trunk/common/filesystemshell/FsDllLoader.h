#ifndef SI_FILESYSTEM_FSDLLLOADER_H
#define SI_FILESYSTEM_FSDLLLOADER_H
//�����ļ�ϵͳ��صĶ�̬�⣬���ӿ��л�ȡ����ĺ���
//����ֻ������һ���ӿڣ���ͬƽ̨���в�ͬ��ʵ�֡�


//��Ҫʵ�ֲ�ͬƽ̨�¶�̬��ļ��ط�ʽ
class CFsDllLoader
{
public:
    CFsDllLoader();
    virtual ~CFsDllLoader();
    void setDllDir(const std::wstring& dllDir);
    //���ض�̬��Ľӿں���
    bool Load();
    //�ͷ������صĿ�
    virtual void  Release();
    //ȡ�ú�����ڵ�ַ
    virtual void* GetFunction(const char* strFuncName);
    class CImpl;
private:
    CImpl* GetImpl();
    CImpl* impl_;
};
//��ͬƽ̨���ض�̬��ĳ���ӿ�
class CFsDllLoader::CImpl
{
public:
    virtual ~CImpl() {}
    CImpl() {}
    //��������ʵ���࣬�ڲ�ͬʵ�ֵ�CPP��ʵ��
    static CImpl* Create();
    void setDllDir(const std::wstring& dllDir) {
        _dllDir = dllDir;
    }
    //���ض�̬��Ľӿں���
    virtual bool Load() = 0;
    //�ͷ������صĿ�
    virtual void  Release() = 0;
    //ȡ�ú�����ڵ�ַ
    virtual void* GetFunction(const char* strFuncName) = 0;
protected:
    void* hModule_;
    std::wstring _dllDir;
};
#endif
