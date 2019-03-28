#include "stdafx.h"
#include "siutility/MacCommonHelper.h"
#include "filesystemshell/FileSystemShell.h"
#include "siutility/FileSearch.h"
#include <unistd.h>

#define MAX_PATH 256

using namespace filesystem;
namespace siutil
{
    std::set<std::wstring> MacCommonHelper::setUserNames_;
//    bool MacCommonHelper::timezoneParsed_ = false;
//    long MacCommonHelper::bias_ = 0;
    std::string MacCommonHelper::tiemzoneRegion_;
    std::mutex MacCommonHelper::tz_mu_;


    MacCommonHelper::MacCommonHelper()
    {

    }

    MacCommonHelper::~MacCommonHelper()
    {

    }
    //===========================================================
    // brief :		获取mac下的所有用户名
    // Returns:
    // Parameter:	setUsers	保存所有用户名
    // Author:    	jiaowei
    // DATE:      	2015.8.3 18:10:18
    //===========================================================
    void MacCommonHelper::GetMacAllUserName(std::set<std::wstring> & setUsers)
    {
        if(setUserNames_.empty())
        {
            std::wstring strAnalysisPath;
            strAnalysisPath = L"/Users";

            std::vector<PSIHANDLE> pathHandles;
            CFileSystem::GetItemHandlesByRawPath(strAnalysisPath, pathHandles);
            for (std::vector<PSIHANDLE>::iterator iter=pathHandles.begin(); iter != pathHandles.end(); ++iter)
            {
                PSIHANDLE singleUserHandle = *iter;
                std::vector<PSIHANDLE> vecHandles;
                CFileSystem::GetAllSubItems(singleUserHandle, vecHandles,false,L"");
                for (size_t i=0; i<vecHandles.size(); ++i)
                {
                    PSIHANDLE subDirHandle = vecHandles.at(i);
                    SPFileItem fileItemInfo;
                    CFileSystem::GetFileItemInfo(subDirHandle, &fileItemInfo);
                    if (fileItemInfo.strName == std::wstring(L".") || fileItemInfo.strName == std::wstring(L".."))
                    {
                        continue;
                    }
                    else if (fileItemInfo.bFolder)
                    {
                        setUserNames_.insert(fileItemInfo.strName);
                    }
                }

            }
        }

        setUsers = setUserNames_;
    }

    void MacCommonHelper::GetHandlesByUserRawPath(const std::wstring& userRawRootPath,
                                                       const std::wstring& fileName,
                                                       std::vector<PSIHANDLE>& handles)
    {
        std::set<std::wstring> userNames;
        MacCommonHelper::GetMacAllUserName(userNames);

        std::wstring newuserrawpath = userRawRootPath;
        if(!userRawRootPath.empty() && userRawRootPath.at(0) == L'/')
        {
            newuserrawpath = userRawRootPath.substr(1);
        }

        for(std::set<std::wstring>::iterator it = userNames.begin(); it != userNames.end(); ++it)
        {
            std::wstring rootRawPath = L"/Users/" + *it + L"/" + newuserrawpath;

            std::vector<PSIHANDLE> temptargetHandles;
            std::wstring targetRegex = fileName + L"$"; //用户名正则
            siutil::CFileSearch cfs(rootRawPath, targetRegex);
            cfs.GetHandles(temptargetHandles);

            handles.insert(handles.end(), temptargetHandles.begin(), temptargetHandles.end());
        }
    }

    // 时区解析不太对，暂不使用, 代码先保留
//    long MacCommonHelper::GetTimezoneBias()
//    {
//        if(!timezoneParsed_)
//        {
//            std::wstring zoneFile = GetTimeZoneFilePath();
//            CMacTimezoneParser timezoneParser;
//            long bias = 0;
//            if(timezoneParser.GetTimeZoneBias(zoneFile, bias))
//            {
//                bias_ = bias;
//                timezoneParsed_ = true;
//            }
//        }
//        return bias_;
//    }

    std::string MacCommonHelper::GetTimeZoneFilePath(PSIHANDLE pVolume)
    {
        std::wstring RawPath = L"/private/etc/localtime";
        PSIHANDLE handle = CFileSystem::GetItemHandleByRawPath(pVolume, RawPath, true);
        if(handle == NULL)
        {
            return "";
        }
        std::wstring path = filesystem::CFileSystem::GetItemPath(handle);

        //由于Linux系统挂载苹果磁盘后，时区信息文件的大小都为0，无法分析
        // /private/etc/localtime -> /usr/share/zoneinfo/*
        // Mac下的时区信息文件（TZfile）和linux下的一致，所以这里直接返回Mac下的路径
        return MacCommonHelper::GetRealPath(path);
    }

    std::string MacCommonHelper::GetRealPath(std::wstring lnkPath)
    {
        char realTempPath[MAX_PATH] = {0};
        std::string realPath;
        std::string lnk = CPCodeChanger::UnicodeToUTF8(lnkPath.c_str());
        int chcount = readlink(lnk.c_str(), realTempPath, MAX_PATH-1);
        if(chcount > 0)
        {
            realPath = realTempPath;
        }

        return realPath;
    }

    std::wstring MacCommonHelper::GetMountPath(PSIHANDLE handle)
    {
        std::wstring itemPath = CFileSystem::GetItemPath(handle);
        std::vector<PSIHANDLE> pVolumes;
        CFileSystem::GetAllVolumes(pVolumes);
        for(std::vector<PSIHANDLE>::iterator it = pVolumes.begin(); it != pVolumes.end(); ++it)
        {
            std::wstring volumePath = CFileSystem::GetItemPath(*it);
            if(itemPath.find(volumePath + L"/") != std::string::npos)
            {
                return volumePath;
            }
        }
        return L"";
    }

    std::string MacCommonHelper::GetTimezoneRegion()
    {
        if(!tiemzoneRegion_.empty())
        {
            return tiemzoneRegion_;
        }

        std::lock_guard<std::mutex> lock(tz_mu_);
        if(!tiemzoneRegion_.empty()) //再判断一次
        {
            return tiemzoneRegion_;
        }

        std::vector<PSIHANDLE> pVolumes;
        CFileSystem::GetAllVolumes(pVolumes);
        for(std::vector<PSIHANDLE>::iterator it = pVolumes.begin(); it != pVolumes.end(); ++it)
        {
            std::string regionName = GetTimezoneRegion(*it);
            if(!regionName.empty())
            {
                tiemzoneRegion_ = regionName;
                return tiemzoneRegion_;
            }
        }
        return "";
    }

    std::string MacCommonHelper::GetTimezoneRegion(PSIHANDLE pVolume)
    {
        std::string regionName;
        std::string zoneFilePath = GetTimeZoneFilePath(pVolume);
        std::string basestr = "/usr/share/zoneinfo/";
        size_t pos = zoneFilePath.find(basestr);
        if(pos != std::string::npos)
        {
            regionName = zoneFilePath.substr(pos + basestr.size());
            return regionName;
        }
        else
            return "";
    }

    bool MacCommonHelper::IsMacSystemVolume(PSIHANDLE pVolume)
    {
        PSIHANDLE handle = NULL;
        handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/System/Library/Coreservices/SystemVersion.plist", true);
        if(NULL == handle)
        {
            return false;
        }
        handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/Users", true);
        if(NULL == handle)
        {
            return false;
        }
        handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/Applications", true);
        if(NULL == handle)
        {
            return false;
        }
        handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/private/var/log/system.log", true);
        if(NULL == handle)
        {
            return false;
        }
        handle = CFileSystem::GetItemHandleByRawPath(pVolume, L"/usr/share/zoneinfo", true);
        if(NULL == handle)
        {
            return false;
        }
        return true;
    }
}
