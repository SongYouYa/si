#pragma once
namespace macbasic {

    struct SystemBaseInfo
    {
        std::wstring  m_wsVersion; //系统版本号
        std::wstring m_wsLanguage; //系统语言
        std::wstring m_wsTimeZone; //系统时区
        std::wstring m_wsVersionName;
        CPTime m_installTime; //安装日期
    };
    typedef std::vector<SystemBaseInfo> SystemBaseInfos;
    class  CPSysBaseInfoAnalyzer
    {
    public:
            CPSysBaseInfoAnalyzer();
            ~CPSysBaseInfoAnalyzer();
            bool analyze(CPRecord* rootRecord,bool isAddXml=true);
            bool ParseVersionInfo(const std::wstring& plistpath, std::wstring& version);
            bool ParseLanguageTimezone(const std::wstring& plistpath, std::wstring& language, std::wstring& timezone);
            bool ParseInstallDate(const std::wstring& plistpath, CPTime& installTime);
            std::wstring getOsVersion(){
                return m_osVersion;
            }
    private:
           std::wstring m_osVersion;
    };
}
