#include "stdafx.h"
#include "PRecentlyUsedItemHelper.h"
namespace macbasic{
RecentlyUsedItemhelper::RecentlyUsedItemhelper()
{

}

RecentlyUsedItemhelper::~RecentlyUsedItemhelper()
{

}

bool RecentlyUsedItemhelper::GetRecentusedItemRecords(CPRecord* rootRecord)
{
    siutil::CPHandleExport che;
    std::vector<PSIHANDLE> plistHandles;
    siutil::MacCommonHelper::GetHandlesByUserRawPath(L"Library/Preferences", L"com.apple.recentitems.plist", plistHandles);
    for(std::vector<PSIHANDLE>::iterator it_h = plistHandles.begin(); it_h != plistHandles.end(); ++it_h)
    {
        std::wstring plistPath = CFileSystem::GetItemPath(*it_h);
        //从路径中截取用户名
        int pos1 = plistPath.find(L"/Users/");
        int  pos2 = plistPath.find(L"/Library/");
        std::wstring useName = plistPath.substr(pos1+7,pos2-pos1-7);

        std::wstring plistExportPath = che.Export(*it_h);
        if (!GetRecentlyUsedItemInformations(plistExportPath))
        {
            continue;
        }
        ALLRecentData::iterator iter;
        std::vector<RecentlyUsedItem>::iterator itervector;
        //无法知道获取的总的数据、进度无法估计
        for ( iter= all_RecentlyUsedrecord.begin();iter != all_RecentlyUsedrecord.end(); ++iter )
        {
            for( itervector= iter->second.begin();itervector != iter->second.end(); ++itervector )
            {
               CPRecord *table =  rootRecord->addChild();
                table->at(L"SIUser") = useName;
                table->at(L"SIType") = iter->first;
                RecentlyUsedItem tempItem = *itervector;
                table->at(L"SIName") = tempItem.m_wsName;
                table->at(L"SIAddress") =tempItem.m_wsAddress;
            }
        }
    }
    return true;
}

bool RecentlyUsedItemhelper::GetRecentlyUsedItemInformations(std::wstring path)
{
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(path.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }
    plistcpp::dictionary_type::iterator itrDict = dict.find("RecentApplications");
    if(itrDict == dict.end())
    {
        return false;
    }
    m_wType = L"应用程序";
    AnalysisArray(itrDict->second);
    //有的版本有此信息，有的没有
    itrDict = dict.find("RecentDocuments");
    if(itrDict != dict.end())
    {
        m_wType = L"文稿";
        AnalysisArray(itrDict->second);

    }

    return true;
}
//解析plistArray
void RecentlyUsedItemhelper::AnalysisArray(boost::any para)
{

    if(!all_visitName.empty())
    {
        all_visitName.clear();
    }
    plistcpp::dictionary_type  plistDic = boost::any_cast<const plistcpp::dictionary_type &>(para);

    plistcpp::dictionary_type::iterator itrsecondDict = plistDic.find("CustomListItems");

    plistcpp::array_type arrayData = boost::any_cast<const plistcpp::array_type&>(itrsecondDict->second);
    plistcpp::array_type::iterator iterArray = arrayData.begin();
    for(; iterArray != arrayData.end(); ++iterArray)
    {
         RecentlyUsedItem singleItem;
         plistcpp::dictionary_type AccountData = boost::any_cast<plistcpp::dictionary_type>(*iterArray);
         plistcpp::dictionary_type::iterator iterName = AccountData.find("Name");

         if(iterName == AccountData.end())
         {
             continue;
         }

         std::string strKeyName;
         strKeyName= boost::any_cast<plistcpp::string_type>(iterName->second);
         singleItem.m_wsName =  CPCodeChanger::UTF8ToUnicode(strKeyName.c_str());
         //获取地址信息，服务器会有此值
          plistcpp::dictionary_type::iterator iterAddress = AccountData.find("URL");
          if(iterAddress != AccountData.end())
          {
              std::string strKeyURL;
              strKeyURL= boost::any_cast<plistcpp::string_type>(iterAddress->second);
              singleItem.m_wsAddress=  CPCodeChanger::UTF8ToUnicode(strKeyURL.c_str());
          }
          all_visitName.push_back(singleItem);

    }
    all_RecentlyUsedrecord[m_wType] = all_visitName;

}
}

