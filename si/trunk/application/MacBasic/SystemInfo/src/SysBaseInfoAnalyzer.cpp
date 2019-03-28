#include "stdafx.h"
#include "SystemInfo/include/systeminfo/SysBaseInfoAnalyzer.h"

using namespace filesystem;
using namespace  siutility;
namespace macbasic {

CSysBaseInfoAnalyzer::CSysBaseInfoAnalyzer()
{

}


CSysBaseInfoAnalyzer::~CSysBaseInfoAnalyzer()
{

}

bool CSysBaseInfoAnalyzer::analyze(CPRecord* rootRecord)
{
    SystemBaseInfos allInfos;
    std::vector<PSIHANDLE> pVolumes;
    CFileSystem::GetAllVolumes(pVolumes);
    for(std::vector<PSIHANDLE>::iterator vit = pVolumes.begin(); vit != pVolumes.end(); ++vit)
    {
        PSIHANDLE pVolume = *vit;
        if(!MacCommonHelper::IsMacSystemVolume(pVolume))
        {
           continue;
        }
        SystemBaseInfo oneInfo;
        bool bSuc = false;

        siutility::CHandleExport che;
        PSIHANDLE handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/System/Library/Coreservices/SystemVersion.plist", true);
        if(handle != NULL)
        {
            std::wstring plistPath = che.Export(handle);
            ParseVersionInfo(plistPath, oneInfo.m_wsVersion);
            bSuc = true;
        }
        PSIHANDLE handle2 = CFileSystem::GetItemHandleByRawPath(pVolume, L"/Library/Preferences/.GlobalPreferences.plist", true);
        if(handle2 != NULL)
        {
            std::wstring plistPath = che.Export(handle2);
            ParseLanguageTimezone(plistPath, oneInfo.m_wsLanguage, oneInfo.m_wsTimeZone);
            bSuc = true;
        }
        PSIHANDLE installHandle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/Library/Receipts/InstallHistory.plist", true);
        if(installHandle != NULL)
        {
            std::wstring plistPath = che.Export(installHandle);
            ParseInstallDate(plistPath, oneInfo.m_installTime);
            bSuc = true;
        }

        if(bSuc)
        {
            allInfos.push_back(oneInfo);
        }
    }

    for (SystemBaseInfos::iterator iter = allInfos.begin(); iter != allInfos.end(); ++iter)
    {
        CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.systembaseinfo");
        CPRecord* osRootRecord = osType->newRecord();
        rootRecord->addChild(osRootRecord);
        CPRecord* osRecord = osRootRecord->addChild();
        SystemBaseInfo& tempOneRecord = *iter;
        osRecord->at(L"SIVersion") = tempOneRecord.m_wsVersion;
        osRecord->at(L"SILanguage") = tempOneRecord.m_wsLanguage;
        osRecord->at(L"SITimeZone") = tempOneRecord.m_wsTimeZone;
        osRecord->at(L"SIInstallTime") = tempOneRecord.m_installTime;
       // qDebug()<<tempOneRecord.m_wsVersion<<endl;
       // qDebug()<<tempOneRecord.m_wsLanguage<<endl;
        return true;
    }
    return false;
}

bool CSysBaseInfoAnalyzer::ParseVersionInfo(const std::wstring& plistpath, std::wstring& version)
{
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(plistpath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }
    plistcpp::dictionary_type::iterator itrDict = dict.find("ProductVersion");
    if(itrDict != dict.end())
    {
        std::string strKeyVersion = boost::any_cast<plistcpp::string_type>(itrDict->second);
        version = CPCodeChanger::UTF8ToUnicode(strKeyVersion.c_str());
        return true;
    }
    return false;
}

bool CSysBaseInfoAnalyzer::ParseLanguageTimezone(const std::wstring &plistpath, std::wstring &language, std::wstring &timezone)
{
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(plistpath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }
    plistcpp::dictionary_type::iterator itrDict = dict.find("macbasicleLanguages");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string strLanguage = boost::any_cast<plistcpp::string_type>(*iterArray);
            language = CPCodeChanger::UTF8ToUnicode(strLanguage.c_str());
            break;
        }
    }

    //获取系统时区
    plistcpp::dictionary_type::iterator itrTimeZoneDict = dict.find("com.macbasicle.preferences.timezone.selected_city");
    if(itrTimeZoneDict != dict.end())
    {
        plistcpp::dictionary_type dict_tz = boost::any_cast<plistcpp::dictionary_type>(itrTimeZoneDict->second);
        plistcpp::dictionary_type::iterator it_tz = dict_tz.find("TimeZoneName");
        if(it_tz != dict_tz.end())
        {
            std::string zoneName = boost::any_cast<plistcpp::string_type>(it_tz->second);
            timezone = CPCodeChanger::UTF8ToUnicode(zoneName.c_str());
        }
    }

    return true;
}

bool CSysBaseInfoAnalyzer::ParseInstallDate(const std::wstring& plistpath,CPTime& installTime)
{
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(plistpath.c_str());
    plistcpp::array_type array;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), array);
    }
    catch(...)
    {
        return false;
    }

    bool bSuc = false;
    time_t inst = 0;
    for(plistcpp::array_type::iterator it = array.begin(); it != array.end(); ++it)
    {
        plistcpp::dictionary_type dict = boost::any_cast<plistcpp::dictionary_type>(*it);
        std::string displayName;
        std::string processName;
        plistcpp::dictionary_type::iterator it_d = dict.find("displayName");
        if(it_d != dict.end())
        {
            displayName = boost::any_cast<std::string>(it_d->second);
        }
        it_d = dict.find("processName");
        if(it_d != dict.end())
        {
            processName = boost::any_cast<std::string>(it_d->second);
        }
        if(displayName.find("OS") != std::string::npos && "OS X Installer" == processName) //"OS  X" == displayName
        {
            it_d = dict.find("date");
            plistcpp::date_type date = boost::any_cast<plistcpp::date_type>(it_d->second);
            time_t t = date.timeAsEpoch();
            if(t > inst) //文件里，可能有多个安装时间，这里取最后一个时间
            {
                inst = t;
            }
            bSuc = true;
        }
    }

    if(bSuc)
    {
        installTime = CPTime::UTC2LocalTime(inst, siutility::MacCommonHelper::GetTimezoneRegion());
    }

    return bSuc;
}

}
