#include "stdafx.h"
#include "PAutorunParser.h"
#include "PLaunchParser.h"

#define MAC_AUTORUN_KEXT_ROOT L"/System/Library/Extensions"
#define ALL_USER_STR L"所有用户"
namespace macbasic {
    CPAutorunParser::~CPAutorunParser()
    {

    }

    bool CPAutorunParser::GetAutorunInfo(CPRecord *table)
    {
        AutoRunInfos allInfos;
        GetKext(allInfos);
        GetLaunchctl(allInfos);
        GetMachInit(allInfos);
        GetLauchDaemons(allInfos);
        GetSystemStarter(allInfos);
        GetLaunchAgent(allInfos);
        GetLoginitems(allInfos);
       // size_t infoCount = allInfos.size();
        AutoRunInfos::iterator it_r = allInfos.begin();
        for(size_t i = 0; it_r != allInfos.end(); ++it_r, ++i)
        {
            CPRecord* fileRecord = table->addChild();
            fileRecord->at(L"Type") = (*it_r).type_;
            fileRecord->at(L"FileName") = (*it_r).fileName_;
            fileRecord->at(L"Command") = (*it_r).command_;
            fileRecord->at(L"User") = (*it_r).user_;
            fileRecord->at(L"Description") = (*it_r).description_;
            fileRecord->at(L"Path") = (*it_r).filePath_;
            fileRecord->at(L"LastAccess") = (*it_r).lastAccessTime_;
            fileRecord->at(L"LastModify") = (*it_r).lastModifyTime_;
            fileRecord->at(L"Remark") =(*it_r). remark_;

        }
        return true;
    }

    bool CPAutorunParser::ParseKext(PSIHANDLE plistHandle, AutoRunInfo& kextInfo)
    {   CPHandleExport fileExport;
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

        plistcpp::dictionary_type::iterator it_d = dict.find("OSBundleRequired");
        if(it_d != dict.end())
        {
            std::string strMode = boost::any_cast<std::string>(it_d->second);
            kextInfo.remark_ = L"加载模式:" + CPCodeChanger::UTF8ToUnicode(strMode.c_str());
        }
        it_d = dict.find("CFBundleGetInfoString");
        if(it_d != dict.end())
        {
            std::string strInfo = boost::any_cast<std::string>(it_d->second);
            kextInfo.description_ = CPCodeChanger::UTF8ToUnicode(strInfo.c_str());
        }
        it_d = dict.find("CFBundleExecutable");
        if(it_d != dict.end())
        {
            std::string strExe = boost::any_cast<std::string>(it_d->second);
            kextInfo.command_ = CPCodeChanger::UTF8ToUnicode(strExe.c_str());
        }

        PSIHANDLE kextHandle = CFileSystem::GetItemParent(plistHandle);
        std::wstring parName = CFileSystem::GetItemName(kextHandle);
        if(parName.find(L".kext") == std::wstring::npos)
        {
            kextHandle = CFileSystem::GetItemParent(kextHandle); // 上两级目录
        }
        kextInfo.fileName_ = CFileSystem::GetItemName(kextHandle);
        PSIHANDLE kextRoot = CFileSystem::GetItemParent(kextHandle);
        kextInfo.filePath_ = CFileSystem::GetItemPath(kextRoot); //kext目录上级目录的路径
        kextInfo.user_ = ALL_USER_STR;
        kextInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(plistHandle);
        kextInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(plistHandle);
        kextInfo.type_ = L"内核扩展模块";

        return true;
    }

    void CPAutorunParser::GetKext(AutoRunInfos& allKextInfo)
    {
        std::vector<PSIHANDLE> pInfoHandles;
        siutil::CFileSearch cfs(MAC_AUTORUN_KEXT_ROOT, L"/Info.plist$");
        cfs.GetHandles(pInfoHandles);
        for(std::vector<PSIHANDLE>::iterator it = pInfoHandles.begin(); it != pInfoHandles.end(); ++it)
        {
            PSIHANDLE handle = *it;
            AutoRunInfo newKextInfo;
            if(ParseKext(handle, newKextInfo))
            {
                allKextInfo.push_back(newKextInfo);
            }
        }
    }

    void CPAutorunParser::GetLaunchctl(AutoRunInfos& launchctlInfos)
    {
        std::vector<std::wstring> rawPath;
        rawPath.push_back(L"/etc/rc.cdrom");
        rawPath.push_back(L"/etc/rc.netboot");
        rawPath.push_back(L"/sbin/fsck" );
        rawPath.push_back(L"/etc/rc.server" );
        rawPath.push_back(L"/var/account/acct" );
        rawPath.push_back(L"/etc/rc.installer_cleanup" );
        rawPath.push_back(L"/etc/security/rc.audit" );
        rawPath.push_back(L"/usr/sbin/BootCacheControl" );
        rawPath.push_back(L"/var/db/BootCache.playlist" );
        rawPath.push_back(L"/etc/launchd.conf" );

        for(std::vector<std::wstring>::iterator it = rawPath.begin(); it != rawPath.end(); ++it)
        {
            std::vector<PSIHANDLE> handles;
            CFileSystem::GetItemHandlesByRawPath(*it, handles);
            for(std::vector<PSIHANDLE>::iterator it_h = handles.begin(); it_h != handles.end(); ++it_h)
            {
                AutoRunInfo newInfo;
                if(ParseLaunchctl(*it_h, newInfo))
                {
                    launchctlInfos.push_back(newInfo);
                }
            }
        }
    }

    bool CPAutorunParser::ParseLaunchctl(PSIHANDLE handle, AutoRunInfo& lauchInfo)
    {
        lauchInfo.fileName_ = CFileSystem::GetItemName(handle);
        lauchInfo.user_ = ALL_USER_STR;
        lauchInfo.filePath_ = CFileSystem::GetItemPath(CFileSystem::GetItemParent(handle));
        lauchInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(handle);
        lauchInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(handle);
        lauchInfo.type_ = L"launchctl调用";

        return true;
    }

     void CPAutorunParser::GetMachInit(AutoRunInfos& machinitInfos)
     {
        std::wstring rootPath = L"/etc/mach_init.d/";
        siutil::CFileSearch cfs(rootPath, L".*");
        std::vector<PSIHANDLE> plistHandles;
        cfs.GetHandles(plistHandles);
        for(std::vector<PSIHANDLE>::iterator it = plistHandles.begin(); it != plistHandles.end(); ++it)
        {
            AutoRunInfo newMachinit;
            if(ParseMachInit(*it, newMachinit))
            {
                machinitInfos.push_back(newMachinit);
            }
        }
     }

     bool CPAutorunParser::ParseMachInit(PSIHANDLE plistHandle, AutoRunInfo& machinitInfo)
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
        plistcpp::dictionary_type::iterator it_d = dict.find("Command");
        if(it_d != dict.end())
        {
            std::string strcom = boost::any_cast<std::string>(it_d->second);
            machinitInfo.command_ = CPCodeChanger::UTF8ToUnicode(strcom.c_str());
        }
        it_d = dict.find("OnDemand");
        if(it_d != dict.end())
        {
            bool bo = boost::any_cast<bool>(it_d->second);
            if(bo)
                machinitInfo.remark_ = L"OnDemand: yes";
            else
                machinitInfo.remark_ = L"OnDemand: no";
        }
        it_d = dict.find("ServiceName");
        if(it_d != dict.end())
        {
            std::string strser = boost::any_cast<std::string>(it_d->second);
            machinitInfo.description_ = CPCodeChanger::UTF8ToUnicode(strser.c_str());
        }
        PSIHANDLE pPar = CFileSystem::GetItemParent(plistHandle);
        machinitInfo.filePath_ = CFileSystem::GetItemPath(pPar);
        machinitInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(plistHandle);
        machinitInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(plistHandle);
        machinitInfo.type_ = L"mach_init.d";

        return true;
     }

     void CPAutorunParser::GetLauchDaemons(AutoRunInfos& lauchDaemonsInfos)
     {
        std::vector<PSIHANDLE> plistHandles;
        std::vector<PSIHANDLE> tempHandles1;
        std::vector<PSIHANDLE> tempHandles2;
        siutil::CFileSearch cfs(L"/Library/LaunchDaemons/", L".plist");
        cfs.GetHandles(tempHandles1);
        siutil::CFileSearch cfs2(L"/System/Library/LaunchDaemons/", L".plist");
        cfs2.GetHandles(tempHandles2);
        plistHandles.insert(plistHandles.end(), tempHandles1.begin(), tempHandles1.end());
        plistHandles.insert(plistHandles.end(), tempHandles2.begin(), tempHandles2.end());

        for(std::vector<PSIHANDLE>::iterator it = plistHandles.begin(); it != plistHandles.end(); ++it)
        {
            AutoRunInfo oneAutorun;
            CPLaunchParser ldp;
            if(ldp.ParseLauchDaemon(*it, oneAutorun))
            {
                oneAutorun.type_ = L"LaunchDaemon";
                lauchDaemonsInfos.push_back(oneAutorun);
            }
        }
     }

     void CPAutorunParser::GetSystemStarter(AutoRunInfos& systemStarterInfos)
     {
         GetSystemStarterProg(systemStarterInfos);
         GetSystemStarterScript(systemStarterInfos);
         GetSystemStarterItems(systemStarterInfos);
     }

     void CPAutorunParser::GetSystemStarterProg(AutoRunInfos& systemStarterInfos)
     {
         std::map<std::wstring, std::wstring> progparams;
         progparams[L"/usr/sbin/ipconfig"] = L"/usr/sbin/ipconfig waitall";
         progparams[L"/sbin/autodiskmount"] = L"/sbin/autodiskmount -va";
         progparams[L"/usr/libexec/dirhelper"] = L"/usr/libexec/dirhelper";
        ParseSystemStarterProg(progparams, systemStarterInfos);
     }

     void CPAutorunParser::ParseSystemStarterProg(std::map<std::wstring, std::wstring> progparams, AutoRunInfos& systemStarterInfos)
     {
        std::map<std::wstring, std::wstring>::iterator it = progparams.begin();
        for(; it != progparams.end(); ++it)
        {
            std::wstring rawpath = it->first;
            std::vector<PSIHANDLE> handles;
            CFileSystem::GetItemHandlesByRawPath(rawpath, handles);
            for(std::vector<PSIHANDLE>::iterator it_h = handles.begin(); it_h != handles.end(); ++it_h)
            {
                PSIHANDLE handle = *it_h;
                AutoRunInfo oneInfo;
                oneInfo.fileName_ = CFileSystem::GetItemName(handle);
                oneInfo.filePath_ = CFileSystem::GetItemPath(CFileSystem::GetItemParent(handle));
                oneInfo.type_ = L"SystemStarter调用程序";
                oneInfo.user_ = ALL_USER_STR;
                oneInfo.command_ = it->second;
                oneInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(handle);
                oneInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(handle);
                systemStarterInfos.push_back(oneInfo);
            }
        }
        return;
     }

     void CPAutorunParser::GetSystemStarterScript(AutoRunInfos& systemStarterInfos)
     {
         std::vector<PSIHANDLE> handles;
         CFileSystem::GetItemHandlesByRawPath(L"/etc/rc.local", handles);
         CFileSystem::GetItemHandlesByRawPath(L"/etc/rc.shutdown.local", handles);

         for(std::vector<PSIHANDLE>::iterator it_h = handles.begin(); it_h != handles.end(); ++it_h)
         {
             PSIHANDLE pItem = *it_h;
             AutoRunInfo oneInfo;
             oneInfo.fileName_ = CFileSystem::GetItemName(pItem);
             oneInfo.filePath_ = CFileSystem::GetItemPath(CFileSystem::GetItemParent(pItem));
             oneInfo.type_ = L"SystemStarter运行脚本";
             oneInfo.user_ = ALL_USER_STR;
             oneInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(pItem);
             oneInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(pItem);
         }
     }

     void CPAutorunParser::GetSystemStarterItems(AutoRunInfos& systemStarterInfos)
     {
         std::vector<PSIHANDLE> rootHandles;
         CFileSystem::GetItemHandlesByRawPath(L"/Library/StartupItems/", rootHandles);
         CFileSystem::GetItemHandlesByRawPath(L"/System/Library/StartupItems/", rootHandles);

         for(std::vector<PSIHANDLE>::iterator it_h = rootHandles.begin(); it_h != rootHandles.end(); ++it_h)
         {
             AutoRunInfos tempInfos;
             if(ParseSystemStarterItems(*it_h, tempInfos))
             {
                 systemStarterInfos.insert(systemStarterInfos.end(), tempInfos.begin(), tempInfos.end());
             }
         }
     }

     bool CPAutorunParser::ParseSystemStarterItems(PSIHANDLE rootHandle, AutoRunInfos& systemStarterInfos)
     {
         std::vector<PSIHANDLE> subHandles;
         CFileSystem::GetAllSubItems(rootHandle, subHandles, false/*, getDirExpr()*/);
         if(subHandles.empty())
         {
             return false;
         }
         for(std::vector<PSIHANDLE>::iterator it = subHandles.begin(); it != subHandles.end(); ++it)
         {
             AutoRunInfo oneInfo;
             oneInfo.filePath_ = CFileSystem::GetItemPath(rootHandle);
             PSIHANDLE pItem = *it;
             oneInfo.fileName_ = CFileSystem::GetItemName(pItem);
             std::wstring strpath = CFileSystem::GetItemPath(pItem);
            if(boost::filesystem::exists(strpath + L"/.disabled"))
            {
                oneInfo.remark_ = L"禁止使用";
            }
            else
            {
                oneInfo.remark_ = L"使用中";
            }
            oneInfo.user_ = ALL_USER_STR;
            oneInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(pItem);
            oneInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(pItem);
            oneInfo.type_ = L"StartupItem";
         }

         return true;
     }

     void CPAutorunParser::GetLaunchAgent(AutoRunInfos& agentInfos)
     {
        std::vector<PSIHANDLE> agentdirs;
        CFileSystem::GetItemHandlesByRawPath(L"/Library/LaunchAgents/", agentdirs);
        CFileSystem::GetItemHandlesByRawPath(L"/System/Library/LaunchAgents/", agentdirs);
        for(std::vector<PSIHANDLE>::iterator it = agentdirs.begin(); it != agentdirs.end(); ++it)
        {
            AutoRunInfos tempInfos;
            if(ParseLaunchAgentRoot(*it, tempInfos))
            {
                agentInfos.insert(agentInfos.end(), tempInfos.begin(), tempInfos.end());
            }
        }

        //用户目录下的
        std::set<std::wstring> userNames;
        siutil::MacCommonHelper::GetMacAllUserName(userNames);
        for(std::set<std::wstring>::iterator it = userNames.begin(); it != userNames.end(); ++it)
        {
            std::wstring username = *it;
            std::wstring rawPath = L"/Users/" + username + L"/Library/LaunchAgents/";
            std::vector<PSIHANDLE> userAgentdirs;
            CFileSystem::GetItemHandlesByRawPath(rawPath, userAgentdirs);
            for(std::vector<PSIHANDLE>::iterator it_h = userAgentdirs.begin(); it_h != userAgentdirs.end(); ++it_h)
            {
                AutoRunInfos tempInfos;
                if(ParseLaunchAgentRoot(*it_h, tempInfos))
                {
                    for(AutoRunInfos::iterator it_i = tempInfos.begin(); it_i != tempInfos.end(); ++it_i)
                    {
                        it_i->user_ = username;
                    }
                    agentInfos.insert(agentInfos.end(), tempInfos.begin(), tempInfos.end());
                }
            }
        }
     }

     bool CPAutorunParser::ParseLaunchAgentRoot(PSIHANDLE rootHandle, AutoRunInfos& agentInfos)
     {
         std::vector<PSIHANDLE> subHandles;
         filesystem::CFileSystem::GetAllSubItems(rootHandle, subHandles, false, L"");
         for(std::vector<PSIHANDLE>::iterator it = subHandles.begin(); it != subHandles.end(); ++it)
         {
             CPLaunchParser clp;
             AutoRunInfo oneInfo;
             if(clp.ParseLauchDaemon(*it, oneInfo))
             {
                 oneInfo.type_ = L"LaunchAgent";
                 agentInfos.push_back(oneInfo);
             }
         }

         return !agentInfos.empty();
     }

     void CPAutorunParser::GetLoginitems(AutoRunInfos& loginInfos)
     {
         std::vector<PSIHANDLE> plistHandles;
         siutil::MacCommonHelper::GetHandlesByUserRawPath(L"/Library/Preferences/", L"com.Apple.loginitems.plist", plistHandles);
         for(std::vector<PSIHANDLE>::iterator it = plistHandles.begin(); it != plistHandles.end(); ++it)
         {
            AutoRunInfos tempInfos;
            ParseLoginitem(*it, tempInfos);
            loginInfos.insert(loginInfos.end(), tempInfos.begin(), tempInfos.end());
         }
     }

     bool CPAutorunParser::ParseLoginitem(PSIHANDLE plistHandle, AutoRunInfos& loginInfos)
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
         plistcpp::dictionary_type::iterator it_d = dict.find("SessionItems");
         if(it_d != dict.end())
         {
             plistcpp::dictionary_type dict_2 = boost::any_cast<plistcpp::dictionary_type>(it_d->second);
             plistcpp::dictionary_type::iterator it_d2 = dict_2.find("CustomListItems");
             if(it_d2 != dict_2.end())
             {
                 plistcpp::array_type arr = boost::any_cast<plistcpp::array_type>(it_d2->second);
                 for(plistcpp::array_type::iterator it_a = arr.begin(); it_a != arr.end(); ++it_a)
                 {
                     AutoRunInfo oneInfo;
                     plistcpp::dictionary_type dict_3 = boost::any_cast<plistcpp::dictionary_type>(*it_a);
                     plistcpp::dictionary_type::iterator it_d3 = dict_3.find("Name");
                     if(it_d3 != dict_3.end())
                     {
                         std::string strvalue = boost::any_cast<std::string>(it_d3->second);
                         oneInfo.description_ = CPCodeChanger::UTF8ToUnicode(strvalue.c_str());
                     }
                     it_d3 = dict_3.find("CustomItemProperties");
                     if(it_d3 != dict_3.end())
                     {
                         plistcpp::dictionary_type dict_4 = boost::any_cast<plistcpp::dictionary_type>(it_d3->second);
                         plistcpp::dictionary_type::iterator it_d4 = dict_4.find("com.Apple.loginitem.legacyprefs");
                         if(it_d4 != dict_4.end())
                         {
                             plistcpp::dictionary_type dict_5 = boost::any_cast<plistcpp::dictionary_type>(it_d4->second);
                             plistcpp::dictionary_type::iterator it_d5 = dict_5.find("Path");
                             if(it_d5 != dict_5.end())
                             {
                                 std::string strvalue = boost::any_cast<std::string>(it_d5->second);
                                 oneInfo.command_ = CPCodeChanger::UTF8ToUnicode(strvalue.c_str());
                             }
                         }
                     }
                     loginInfos.push_back(oneInfo);
                 }
             }
         }

        std::wstring strpath = CFileSystem::GetItemPath(plistHandle);
        size_t ps = strpath.find(L"/Users/");
        size_t pe = strpath.find(L"/Library/Preferences/");
        std::wstring userName = strpath.substr(ps+7, pe-ps-7);

         for(AutoRunInfos::iterator it = loginInfos.begin(); it != loginInfos.end(); ++it)
         {
             AutoRunInfo& oneInfo = *it;
             oneInfo.fileName_ = CFileSystem::GetItemName(plistHandle);
             oneInfo.filePath_ = CFileSystem::GetItemPath(CFileSystem::GetItemParent(plistHandle));
             oneInfo.lastAccessTime_ = CFileSystem::GetItemLastAccessTime(plistHandle);
             oneInfo.lastModifyTime_ = CFileSystem::GetItemModfiyTime(plistHandle);
             oneInfo.type_ = L"LoginItem";
             oneInfo.user_ = userName;
         }
         return true;
     }
}
