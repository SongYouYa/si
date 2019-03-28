#ifndef SYSBASEINFOANALYZER_H
#define SYSBASEINFOANALYZER_H

namespace macbasic {

    struct SystemBaseInfo
    {
        std::wstring  m_wsVersion; //系统版本号
        std::wstring m_wsLanguage; //系统语言
        std::wstring m_wsTimeZone; //系统时区
        CPTime m_installTime; //安装日期
    };
    typedef std::vector<SystemBaseInfo> SystemBaseInfos;
    class  CSysBaseInfoAnalyzer
    {
    public:
            CSysBaseInfoAnalyzer();
            ~CSysBaseInfoAnalyzer();
           static bool analyze(CPRecord* rootRecord);
           static bool ParseVersionInfo(const std::wstring& plistpath, std::wstring& version);
           static bool ParseLanguageTimezone(const std::wstring& plistpath, std::wstring& language, std::wstring& timezone);
           static bool ParseInstallDate(const std::wstring& plistpath, CPTime& installTime);
    };
}

#endif //SYSBASEINFOANALYZER_H
