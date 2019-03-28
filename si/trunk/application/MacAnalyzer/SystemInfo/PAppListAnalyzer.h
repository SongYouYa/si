#pragma once
#define AppStoreInstall L"AppStore"
#define NormalInstall   L"普通安装"
#define SystemInternal  L"系统内置"

namespace macbasic {

struct OSXApp
{
    std::wstring  wstrName;
    std::wstring  wstrVersion;
    std::wstring  wstrPath;
    std::wstring  wstrInfoString;
    CPTime  modifyTime;
    CPTime  lastReadTime;
    CPTime  lastStatusTime;
    std::wstring  installSource = NormalInstall;
};

struct InstallInfo {
    std::wstring installSource;
    std::wstring wstrVersion;
};


class CPAppListAnalyzer
{
    public:
        CPAppListAnalyzer();
        ~CPAppListAnalyzer();

        const OSXApp * FirstChild();
        const OSXApp * NextChild();
        bool AddApp(PSIHANDLE AppDirHandle);
        bool PlistAnalyzer(const std::wstring & wstrFile, OSXApp * pApp);

        //install plist 文件分析器
        static bool ParseInstallHistory(const std::wstring & wstrFile);

    private:
        void Clear();

        std::vector<OSXApp *>::const_iterator m_itCurrent; //当前使用节点
        std::vector<OSXApp *> m_vectorApp; //应用程序列表
        std::vector<std::wstring> m_vectorPlist; //应用程序文件列表

        typedef std::map<std::wstring, InstallInfo> MapInstallInfo;
        static MapInstallInfo m_mapInstall;
};

}
