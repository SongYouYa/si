#include "stdafx.h"
#include "PLaunchParser.h"

using namespace filesystem;

namespace macbasic {

    CPLaunchParser::CPLaunchParser()
    {

    }

    CPLaunchParser::~CPLaunchParser()
    {

    }

    bool CPLaunchParser::ParseLauchDaemon(PSIHANDLE plistHandle, AutoRunInfo& lauchDaemonInfo)
    {
        CPHandleExport fileExport;
        std::wstring plistPath = fileExport.Export(plistHandle);
        std::string strPath = CPCodeChanger::UnicodeToUTF8(plistPath.c_str());
        plistcpp::dictionary_type dict;
        try
        {
            plistcpp::CPlistCpp::ReadPlist(strPath.c_str(), dict);
        }
        catch(...)
        {
            return false;
        }

        lauchDaemonInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(plistHandle);
        lauchDaemonInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(plistHandle);
        lauchDaemonInfo.user_ = ALL_USER_STR;
        lauchDaemonInfo.fileName_ = CFileSystem::GetItemName(plistHandle);
        lauchDaemonInfo.filePath_ = CFileSystem::GetItemPath(CFileSystem::GetItemParent(plistHandle));

        try
        {
            plistcpp::dictionary_type::iterator it_d = dict.find("Label");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                lauchDaemonInfo.description_ = CPCodeChanger::UTF8ToUnicode(strvalue.c_str());
            }
            it_d = dict.find("Program");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                lauchDaemonInfo.command_ = CPCodeChanger::UTF8ToUnicode(strvalue.c_str());
            }
            it_d = dict.find("ProgramArguments");
            if(it_d != dict.end())
            {
                plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
                for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                {
                    std::string strarg = boost::any_cast<std::string>(*it_a);
                    lauchDaemonInfo.command_.append(L" " + CPCodeChanger::UTF8ToUnicode(strarg.c_str()));
                }
            }
            it_d = dict.find("Disabled");
            if(it_d != dict.end())
            {
                bool bd = boost::any_cast<bool>(it_d->second);
                NoteBool("Disabled", bd, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("UserName");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                NoteStr("UserName", strvalue, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("GroupName");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                NoteStr("GroupName", strvalue, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("LaunchOnlyOnce");
            if(it_d != dict.end())
            {
                bool bd = boost::any_cast<bool>(it_d->second);
                NoteBool("LaunchOnlyOnce", bd, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("LimitLoadToHosts");
            if(it_d != dict.end())
            {
                plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
                for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                {
                    std::string strvalue = boost::any_cast<std::string>(*it_a);
                    NoteStr("LimitLoadToHosts", strvalue, lauchDaemonInfo.remark_);
                }
            }
            it_d = dict.find("LimitLoadFromHosts");
            if(it_d != dict.end())
            {
                plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
                for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                {
                    std::string strvalue = boost::any_cast<std::string>(*it_a);
                    NoteStr("LimitLoadFromHosts", strvalue, lauchDaemonInfo.remark_);
                }
            }
            it_d = dict.find("LimitLoadToSessionType");
            if(it_d != dict.end())
            {
                if(IsTypeString(it_d->second))
                {
                    std::string strvalue = boost::any_cast<std::string>(it_d->second);
                    NoteStr("LimitLoadToSessionType", strvalue, lauchDaemonInfo.remark_);
                }
                else if(IsTypeArray(it_d->second))
                {
                    plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
                    for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                    {
                        std::string strvalue = boost::any_cast<std::string>(*it_a);
                        NoteStr("LimitLoadToSessionType", strvalue, lauchDaemonInfo.remark_);
                    }
                }
            }
            it_d = dict.find("OnDemand");
            if(it_d != dict.end())
            {
                bool bd = boost::any_cast<bool>(it_d->second);
                NoteBool("OnDemand", bd, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("KeepAlive");
            if(it_d != dict.end())
            {
                if(IsTypeBool(it_d->second))
                {
                    bool bl = boost::any_cast<bool>(it_d->second);
                    NoteBool("KeepAlive", bl, lauchDaemonInfo.remark_);
                }
                else if(IsTypeDict(it_d->second))
                {
                    plistcpp::dictionary_type dict_ka = boost::any_cast<plistcpp::dictionary_type>(it_d->second);
                    plistcpp::dictionary_type::iterator it_ka = dict_ka.find("SuccessfulExit");
                    if(it_ka != dict_ka.end())
                    {
                        bool bd = boost::any_cast<bool>(it_ka->second);
                        NoteBool("SuccessfulExit", bd, lauchDaemonInfo.remark_);
                    }
                    it_ka = dict_ka.find("NetworkState");
                    if(it_ka != dict_ka.end())
                    {
                        bool bd = boost::any_cast<bool>(it_ka->second);
                        NoteBool("NetworkState", bd, lauchDaemonInfo.remark_);
                    }
                    it_ka = dict_ka.find("PathState");
                    if(it_ka != dict_ka.end())
                    {
                        if(IsTypeDict(it_ka->second))
                        {
                            plistcpp::dictionary_type dict_ps = boost::any_cast<plistcpp::dictionary_type>(it_ka->second);
                            for(plistcpp::dictionary_type::iterator it_ps = dict_ps.begin(); it_ps != dict_ps.end(); ++it_ps)
                            {
                                bool bl = boost::any_cast<bool>(it_ps->second);
                                std::string strkey = "PathState::";
                                strkey += boost::any_cast<std::string>(it_ps->first);
                                NoteBool(strkey, bl, lauchDaemonInfo.remark_);
                            }
                        }
                    }
                    it_ka = dict_ka.find("OtherJobEnabled");
                    if(it_ka != dict_ka.end())
                    {
                        if(IsTypeDict(it_ka->second))
                        {
                            plistcpp::dictionary_type dict_oje = boost::any_cast<plistcpp::dictionary_type>(it_ka->second);
                            for(plistcpp::dictionary_type::iterator it_oje = dict_oje.begin(); it_oje != dict_oje.end(); ++it_oje)
                            {
                                bool bl = boost::any_cast<bool>(it_oje->second);
                                std::string strkey = "OtherJobEnabled::";
                                strkey += boost::any_cast<std::string>(it_oje->first);
                                NoteBool(strkey, bl, lauchDaemonInfo.remark_);
                            }
                        }
                    }
                }
            }
            it_d = dict.find("RootDirectory");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                NoteStr("RootDirectory", strvalue, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("StartOnMount");
            if(it_d != dict.end())
            {
                bool bd = boost::any_cast<bool>(it_d->second);
                NoteBool("StartOnMount", bd, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("StartInteval");
            if(it_d != dict.end())
            {
                int64 iv = boost::any_cast<plistcpp::integer_type>(it_d->second);
                NoteInt("StartInteval", iv, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("TimeOut");
            if(it_d != dict.end())
            {
                int64 iv = boost::any_cast<plistcpp::integer_type>(it_d->second);
                NoteInt("TimeOut", iv, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("WorkingDirectory");
            if(it_d != dict.end())
            {
                std::string strvalue = boost::any_cast<std::string>(it_d->second);
                NoteStr("WorkingDirectory", strvalue, lauchDaemonInfo.remark_);
            }
            it_d = dict.find("QueueDirectaries");
            if(it_d != dict.end())
            {
                plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d->second);
                for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                {
                    std::string strvalue = boost::any_cast<std::string>(*it_a);
                    NoteStr("QueueDirectaries", strvalue, lauchDaemonInfo.remark_);
                }
            }
        }
        catch(...)
        {
            return false;
        }

        return true;
    }

    void CPLaunchParser::NoteBool(std::string keyName, bool bValue, std::wstring& note)
    {
        std::wstring wName = CPCodeChanger::UTF8ToUnicode(keyName.c_str());
        note.append(wName);
       if(bValue)
       {
           note.append(L":yes; ");
       }
       else
       {
           note.append(L":no; ");
       }
    }
    void CPLaunchParser::NoteStr(std::string keyName, std::string strValue, std::wstring& note)
    {
       std::wstring wName = CPCodeChanger::UTF8ToUnicode(keyName.c_str());
       std::wstring wValue = CPCodeChanger::UTF8ToUnicode(strValue.c_str());
       note.append(wName + L":" + wValue + L"; ");
    }
    void CPLaunchParser::NoteInt(std::string keyName, int64 intv, std::wstring& note)
    {
        std::wstring wName = CPCodeChanger::UTF8ToUnicode(keyName.c_str());
       std::wstring wstrv = boost::lexical_cast<std::wstring>(intv);
       note.append(wName + L":" + wstrv + L"; ");
    }
     bool CPLaunchParser::IsTypeString(boost::any& value)
     {
         try{
             std::string str = boost::any_cast<std::string>(value);
             return true;
         }
         catch(...)
         {
             return false;
         }
     }
     bool CPLaunchParser::IsTypeArray(boost::any& value)
     {
         try{
             plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(value);
             return true;
         }
         catch(...)
         {
             return false;
         }
     }
     bool CPLaunchParser::IsTypeBool(boost::any& value)
     {
         try{
             bool bl = boost::any_cast<bool>(value);
             return true;
         }
         catch(...)
         {
             return false;
         }
     }
     bool CPLaunchParser::IsTypeDict(boost::any& value)
     {
         try{
             plistcpp::dictionary_type dict = boost::any_cast<plistcpp::dictionary_type>(value);
             return true;
         }
         catch(...)
         {
             return false;
         }
     }

}
