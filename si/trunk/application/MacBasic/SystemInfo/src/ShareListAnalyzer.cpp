#include "stdafx.h"
#include "SystemInfo/include/systeminfo/ShareListAnalyzer.h"
#include "filesystemshell/FileSystemShell.h"
#include "siutility/FileExport.h"
#include <QFileInfo>
using namespace filesystem;

namespace macbasic
{

ShareListAnalyzer::ShareListAnalyzer()
{

}

ShareListAnalyzer::~ShareListAnalyzer()
{

}

bool ShareListAnalyzer::analyze(CPRecord *rootRecord)
{
    GetFileShare();
    GetWebShare();
    GetBlutoothShare();
    WriteDataInTable(rootRecord,m_FileShared);
    return true;
}

void ShareListAnalyzer::GetFileShare()
{
    siutility::CHandleExport che;
    std::vector<PSIHANDLE> pathHandles;
    CFileSystem::GetItemHandlesByRawPath(L"/private/var/db/dslocal/nodes/Default/sharepoints/*.plist", pathHandles);
    for (std::vector<PSIHANDLE>::iterator iter = pathHandles.begin();
         iter != pathHandles.end(); ++iter)
    {
        PSIHANDLE subHandle = *iter;
        std::wstring plistPath = che.Export(subHandle);
        AnalysisFileShareList(plistPath);
    }
}

void ShareListAnalyzer::GetWebShare()
{
    siutility::CHandleExport che;
    std::vector<PSIHANDLE> pathHandleWeb;
    CFileSystem::GetItemHandlesByRawPath(L"/private/etc/apache2/users/*.conf", pathHandleWeb);
    for (std::vector<PSIHANDLE>::iterator iter = pathHandleWeb.begin();
         iter != pathHandleWeb.end(); ++iter)
    {
        PSIHANDLE subHandle = *iter;
        std::wstring pWebPath = che.Export(subHandle);
        AnalysisWebShareList(pWebPath);
    }
}

void ShareListAnalyzer::GetBlutoothShare()
{
    siutility::CHandleExport che;
    std::vector<PSIHANDLE> pathHandleBluetooth;
    CFileSystem::GetItemHandlesByRawPath(L"/Users/*/Library/Preferences/ByHost/*Bluetooth*.plist", pathHandleBluetooth);
    for (std::vector<PSIHANDLE>::iterator iter = pathHandleBluetooth.begin();
         iter != pathHandleBluetooth.end(); ++iter)
    {
        PSIHANDLE subHandle = *iter;
        std::wstring plistPath = che.Export(subHandle);
        std::wstring Username =  SplitUserName(plistPath);
        AnalysisBlutoothShareList(plistPath,Username);
    }
}

bool ShareListAnalyzer::AnalysisFileShareList(std::wstring filePath)
{
    ShareList FileRecoed;
    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(filePath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }

    plistcpp::dictionary_type::iterator itrDict = dict.find("directory_path");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<const plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string strFileName;
            strFileName= boost::any_cast<plistcpp::string_type>(*iterArray);
            std::wstring tempFileName = CPCodeChanger::UTF8ToUnicode(strFileName.c_str());

            int  pos = filePath.find(L"/private/var/db/dslocal/nodes/Default/sharepoints/");
            std::wstring mountpath = filePath.substr(0,pos);

            FileRecoed.wstrSharedAbsoPath .push_back(tempFileName);
            FileRecoed.wstrUser = SplitUserName(tempFileName);
            mountpath =mountpath+tempFileName;
            FileRecoed.wstrLimitAuth =  GetFilePermission(mountpath);
        }
    }
    itrDict = dict.find("afp_name");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<const plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string strAfpName;
            strAfpName= boost::any_cast<plistcpp::string_type>(*iterArray);
            std::wstring tempName = L"afp:";
            tempName += CPCodeChanger::UTF8ToUnicode(strAfpName.c_str());
            tempName +=L";  ";
            FileRecoed.wstrSharedName.push_back(tempName);
        }

    }

    itrDict = dict.find("smb_name");
    if(itrDict != dict.end())
    {
        plistcpp::array_type arrayData = boost::any_cast<const plistcpp::array_type&>(itrDict->second);
        plistcpp::array_type::iterator iterArray = arrayData.begin();
        for(; iterArray != arrayData.end(); ++iterArray)
        {
            std::string strSmbName;
            strSmbName= boost::any_cast<plistcpp::string_type>(*iterArray);
            std::wstring tempName = L"smb:";
            tempName += CPCodeChanger::UTF8ToUnicode(strSmbName.c_str());
            FileRecoed.wstrSharedName.push_back(tempName);
        }
    }

    FileRecoed.wstrType = L"文件共享";
    m_FileShared.push_back(FileRecoed);
    return true;

}

void ShareListAnalyzer::AnalysisWebShareList(std::wstring webPath)
{
    ShareList WebRecord;
    WebRecord.wstrType = L"Web共享";
    std::string temp = CPCodeChanger::UnicodeToUTF8(webPath.c_str());


    FILE * fp = fopen(temp.c_str(), "r");
    if (NULL == fp) {
        return;
    }
    const int SIZE = 512;
    char szBuffer[SIZE + 1] = {0};
    while(!feof(fp)) {
        memset(szBuffer, 0, SIZE);
        fgets(szBuffer, SIZE, fp);

        if (NULL != strstr(szBuffer, "Directory"))
        {
            std::pair<std::string, std::string> value = SplitValue(szBuffer, "/Users/");

            std::string strFileName = value.second;
            if(!strFileName.empty())
            {
                WebRecord.wstrSharedAbsoPath.push_back(CPCodeChanger::UTF8ToUnicode(strFileName.c_str()));
                std::wstring tempStr = SplitUserName(CPCodeChanger::UTF8ToUnicode(strFileName.c_str()));
                WebRecord.wstrUser = tempStr;
                m_FileShared.push_back(WebRecord);

            }
        }
    }
}

std::pair<std::string, std::string> ShareListAnalyzer::SplitValue( std::string  strLine, std::string cDelim)
{
    size_t stPos = strLine.find(cDelim);
    if (stPos != std::string::npos) {
        int nlength = strLine.length();
        std::string temp = strLine.substr(stPos,nlength-5-stPos);
        return std::make_pair(strLine.substr(0, stPos), temp);
    }
    return std::make_pair(strLine, "");
}

bool ShareListAnalyzer::AnalysisBlutoothShareList(std::wstring lpath,std::wstring userName)
{
    ShareList BluetoothRecord;
    BluetoothRecord.wstrType = L"蓝牙共享";
    BluetoothRecord.wstrUser = userName;


    std::string strFilePath = CPCodeChanger::UnicodeToUTF8(lpath.c_str());
    plistcpp::dictionary_type dict;
    try
    {
        plistcpp::CPlistCpp::ReadPlist(strFilePath.c_str(), dict);
    }
    catch(...)
    {
        return false;
    }

    plistcpp::dictionary_type::iterator itrDict = dict.find("OBEXFTPRootFolderLocation");
    bool empty = false;
    std::wstring pathInfo;
    if(itrDict != dict.end())
    {
        std::string strFilename;
        strFilename= boost::any_cast<plistcpp::string_type>(itrDict->second);
        pathInfo = L"共享:";
        pathInfo += CPCodeChanger::UTF8ToUnicode(strFilename.c_str());
        empty = true;

    }
    bool empty2 = false;
    itrDict = dict.find("OBEXPIMDataSaveToLocation");
    if(itrDict != dict.end())
    {
        std::string strFilename;
        strFilename= boost::any_cast<plistcpp::string_type>(itrDict->second);
        pathInfo = L"下载:";
        pathInfo += CPCodeChanger::UTF8ToUnicode(strFilename.c_str());
        empty2 = true;

    }
    if(empty || empty2)
    {
        BluetoothRecord.wstrSharedAbsoPath.push_back(pathInfo);
        m_FileShared.push_back(BluetoothRecord);
    }

}

void ShareListAnalyzer::WriteDataInTable(CPRecord *rootRecord, std::vector<ShareList> &vecData)
{
    for(unsigned int i = 0; i < vecData.size(); i++)
    {
        ShareList onelist = vecData[i];
        std::wstring tempFileName;
        for(std::list<std::wstring>::iterator it = (onelist.wstrSharedAbsoPath).begin();
            it != (onelist.wstrSharedAbsoPath).end(); it++)
        {
            tempFileName += *it;
        }

        std::wstring tempSharedName;
        for(std::list<std::wstring>::iterator it2 = ( onelist.wstrSharedName).begin();
            it2 != (onelist.wstrSharedName).end(); it2++)
        {
            tempSharedName += *it2;
        }
        CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.sharelistinfo");
        CPRecord* osRootRecord = osType->newRecord();
        rootRecord->addChild(osRootRecord);
        CPRecord* osRecord = osRootRecord->addChild();

        osRecord->at(L"SIFileName") = tempFileName;
        osRecord->at(L"SISharedName") = tempSharedName;
        osRecord->at(L"SIUser") = vecData[i].wstrUser;
        osRecord->at(L"SILimits") = vecData[i].wstrLimitAuth;
        osRecord->at(L"SIType") = vecData[i].wstrType;

    }
}

std::wstring ShareListAnalyzer::SplitUserName(std::wstring longStr)
{
    int pos = longStr.find(L"/Users/");
    longStr = longStr.substr(pos+7);
    int pos2 = longStr.find(L"/");
    longStr = longStr.substr(0,pos2);
    return longStr;
}

std::wstring ShareListAnalyzer::GetFilePermission(std::wstring FilePath)
{
    QFileInfo fi(QString::fromStdWString(FilePath));
    QFile::Permissions md = fi.permissions();
    const int SIZE = 50;
    wchar_t wstrMode[SIZE + 1] = {0};
    const char cSplit = '-';
    swprintf(wstrMode, SIZE, L"%c%c%c%c%c%c%c%c%c",
             md & QFile::ReadOwner ? 'r' : cSplit,
             md & QFile::WriteOwner ? 'w' : cSplit,
             md & QFile::ExeOwner ? 'x' : cSplit,
             md & QFile::ReadGroup ? 'r' : cSplit,
             md & QFile::WriteGroup ? 'w' : cSplit,
             md & QFile::ExeGroup ? 'x' : cSplit,
             md & QFile::ReadOther ? 'r' : cSplit,
             md & QFile::WriteOther ? 'w' : cSplit,
             md & QFile::ExeOther ? 'x' : cSplit);
    return wstrMode;

}


}

