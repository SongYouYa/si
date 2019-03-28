#include "stdafx.h"
#include "PUseInfoAnalyzer.h"

using namespace filesystem;
namespace macbasic {

CPUseInfoAnalyzer::CPUseInfoAnalyzer()
{

}

CPUseInfoAnalyzer::~CPUseInfoAnalyzer()
{

}

bool CPUseInfoAnalyzer::GetUseInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> groupPlistHandles;
    CPHandleExport fileExport;
    CFileSystem::GetItemHandlesByRawPath(L"/private/var/db/dslocal/nodes/Default/groups/*.plist", groupPlistHandles);
    for (std::vector<PSIHANDLE>::iterator iter = groupPlistHandles.begin(); iter != groupPlistHandles.end(); ++iter)
    {
        PSIHANDLE groupHandle = *iter;
       std::wstring plistPath = fileExport.Export(groupHandle);
        ReadGroupPlistFile(plistPath);
    }

    std::vector<PSIHANDLE> userPlistHandles;
    CFileSystem::GetItemHandlesByRawPath(L"/private/var/db/dslocal/nodes/Default/users/*.plist", userPlistHandles);
    for (std::vector<PSIHANDLE>::iterator iter = userPlistHandles.begin(); iter != userPlistHandles.end(); ++iter)
    {
        PSIHANDLE userHandle = *iter;
        std::wstring plistPath = fileExport.Export(userHandle);
        ReadUserPlistFile(plistPath,userHandle);
    }

    WriteDataInTable(rootRecord,m_vecUser);
    return true;
}

bool CPUseInfoAnalyzer::ReadGroupPlistFile(std::wstring plistPath)
{
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(plistPath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }

    GROUP group;
    plistcpp::dictionary_type::iterator itrDict = dict.find("name");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        if(iterArray != arrayData.end())
        {
            std::string groupName = boost::any_cast<plistcpp::string_type>(*iterArray);
            group.wstrGroupName =  CPCodeChanger::UTF8ToUnicode(groupName.c_str());
        }
    }

    itrDict = dict.find("gid");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string groupID = boost::any_cast<plistcpp::string_type>(*iterArray);
            group.wstrGID =  CPCodeChanger::UTF8ToUnicode(groupID.c_str());
        }
    }

    itrDict = dict.find("users");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string users = boost::any_cast<plistcpp::string_type>(*iterArray);
            group.listWstrUserName.push_back(CPCodeChanger::UTF8ToUnicode(users.c_str()));
        }
    }
    m_vecGroup.push_back(group);
    return true;
}

bool CPUseInfoAnalyzer::ReadUserPlistFile(std::wstring plistPath,PSIHANDLE handle)
{
    USER user;
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(plistPath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }

    plistcpp::dictionary_type::iterator itrDict = dict.find("name");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        if(iterArray != arrayData.end())
        {
            std::string UserName = boost::any_cast<plistcpp::string_type>(*iterArray);
            std::wstring wstruserName = CPCodeChanger::UTF8ToUnicode(UserName.c_str());
            user.wstrUserName = wstruserName;
            user.wstrGroupName = GetGroupName(user.wstrUserName);//根据用户名获取组名
        }
    }
    else
    {
        return false; //无用户名，不提取
    }

    itrDict = dict.find("uid");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string UserID = boost::any_cast<plistcpp::string_type>(*iterArray);
            user.wstrUID=  CPCodeChanger::UTF8ToUnicode(UserID.c_str());
        }
    }

    itrDict = dict.find("gid");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string users = boost::any_cast<plistcpp::string_type>(*iterArray);
            user.wstrGID = CPCodeChanger::UTF8ToUnicode(users.c_str());
        }
    }

    itrDict = dict.find("home");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string home = boost::any_cast<plistcpp::string_type>(*iterArray);
            std::wstring wstrhome =  CPCodeChanger::UTF8ToUnicode(home.c_str());
            user.wstrHome = wstrhome;
        }
    }

    itrDict = dict.find("realname");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string realName = boost::any_cast<plistcpp::string_type>(*iterArray);
            user.wstrRealName =  CPCodeChanger::UTF8ToUnicode(realName.c_str());
        }
    }

    user.timeCreated = CFileSystem::GetItemCreateTime(handle);
    if(user.wstrHomeLoc.empty())
    {
        user.wstrHomeLoc = L"未启用filevault";
    }

    user.wstrUserType = GetUserType(user);

    m_vecUser.push_back(user);

    return true;
}

std::wstring CPUseInfoAnalyzer::GetGroupName(const std::wstring &wstrUserName)
{
    std::wstring groupName;
    for (unsigned int i = 0; i < m_vecGroup.size(); i++)
    {
        for(std::list<std::wstring>::iterator it = ( m_vecGroup[i].listWstrUserName).begin();
            it != (m_vecGroup[i].listWstrUserName).end(); it++)
        {
            if(wstrUserName == *it)
            {
                std::wstring temp =   m_vecGroup[i].wstrGroupName;
                groupName += temp+L";  ";
                break;
            }
        }
    }
    return groupName;
}

std::wstring CPUseInfoAnalyzer::GetUserType(USER user)
{
    if(user.wstrGID == L"20" ||user.wstrGID  == L"0" ||user.wstrGID  == L"80" || user.wstrGID  == L"201")
    {
        if(user.wstrGID  == L"80")
        {
            return  L"管理员用户";
        }
        else if(user.wstrGID  == L"201")
        {
            return  L"客人用户";
        }
        else
        {
            GROUP *pGroupStaff = GetGroupByGID(L"20");
            if(pGroupStaff && !findUserInGroup(user.wstrUserName, pGroupStaff))
            {
                return L"系统内建用户";
            }

            GROUP *pGroupWheel = GetGroupByGID(L"0");
            if(pGroupWheel && findUserInGroup(user.wstrUserName, pGroupWheel))
            {
                return L"根用户";
            }

            GROUP *pGroupAdmin = GetGroupByGID(L"80");
            if(pGroupAdmin && findUserInGroup(user.wstrUserName, pGroupAdmin))
            {
                return L"管理员用户";
            }

            if(L"/dev/null" == user.wstrHome)
            {
                return L"共享帐户";
            }
            return L"一般用户";
        }
    }

    return L"";
}

GROUP *CPUseInfoAnalyzer::GetGroupByGID(const std::wstring wstrGID)
{
    for(std::vector<GROUP>::iterator it = m_vecGroup.begin();
        it != m_vecGroup.end(); it++)
    {
        if ((*it).wstrGID == wstrGID)
        {
            return &(*it);
        }
    }

    return NULL;
}

bool CPUseInfoAnalyzer::findUserInGroup(const std::wstring wstrUserName, GROUP *pGroup)
{
    for(std::list<std::wstring>::iterator it = (pGroup->listWstrUserName).begin();
        it != (pGroup->listWstrUserName).end(); it++)
    {
        if ((*it) == wstrUserName)
        {
            return true;
        }
    }

    return false;
}

void CPUseInfoAnalyzer::WriteDataInTable( CPRecord* rootRecord,std::vector<USER> &vecUser)
{  
    // table.userinfo

    for(unsigned int i = 0; i < vecUser.size(); i++)
    {
         CPRecord* record = rootRecord->addChild();
        record->at(L"SIUserID") = vecUser[i].wstrUID;
        record->at(L"SIUseName") = vecUser[i].wstrUserName;
        record->at(L"SIUseType") = vecUser[i].wstrUserType;
        record->at(L"SIGroupID") = vecUser[i].wstrGID;
        record->at(L"SIGroupName") = vecUser[i].wstrGroupName;
        record->at(L"SIMainDire") = vecUser[i].wstrHome;
        record->at(L"SICreateTime") = vecUser[i].timeCreated;
        record->at(L"SIFileVaultDire") = vecUser[i].wstrHomeLoc;
        record->at(L"SIReMark") = vecUser[i].wstrRealName;
    }
}
}


