#include "stdafx.h"
#include "PSkypeHelper.h"
#include "PSkypeAnalyzer.h"
#include "filesystemshell/PInterfaceStruct.h"
#define  SKYPEDIR  L"/Library/Application Support/Skype/"
#define  SKYPEFILENAME L"main.db"

using namespace siutil;
using namespace filesystem;
namespace macbasic{

CPSkypeHelper::CPSkypeHelper()
{

}
CPSkypeHelper::~CPSkypeHelper()
{

}

/*get infor mation about rootRecord*/
bool CPSkypeHelper::AnalyzerSkypeAccount(CPRecord *rootRecord)
{
    std::vector<std::wstring> vecAllFiles;
    CPMacFileExport exportFile(SKYPEDIR, SKYPEFILENAME);
    exportFile.GetExportFiles(vecAllFiles);

    std::vector<SkypeUserRecord> vecUserRecords;
    for (std::vector<std::wstring>::iterator it = vecAllFiles.begin();it != vecAllFiles.end(); ++it)
    {
        CPSkypeAnalyzer analyzer;
        analyzer.GetSkypeUserInfomation(*it, vecUserRecords);
    }
    for (std::vector<SkypeUserRecord>::iterator iter = vecUserRecords.begin();
         iter != vecUserRecords.end(); ++iter )
    {
        CPRecord* osRecord = rootRecord->addChild();
        osRecord->at(L"UserName") = (*iter).m_strUserName;
        osRecord->at(L"DisplayName") = (*iter).m_strDisplayName;
        osRecord->at(L"SignName") = (*iter).m_strSignName;
        osRecord->at(L"Country") = (*iter).m_strCountryCode;
        osRecord->at(L"Province") = (*iter).m_strProvince;
        osRecord->at(L"City") = (*iter).m_strCity;
        osRecord->at(L"Sex") = (*iter).m_strSex;
        osRecord->at(L"Birthday") = (*iter).m_strbirthday;
        osRecord->at( L"Phone") = (*iter).m_strPhoneNumber;
        osRecord->at(L"OfficePhone") = (*iter).m_strOfficeNumber;
        osRecord->at(L"Mobile") = (*iter).m_strMobileNumber;
        osRecord->at(L"Email") = (*iter).m_strEmail;
        osRecord->at(L"Website") = (*iter).m_strMainHome;
        osRecord->at(L"About") = (*iter).m_strAbout;

    }
    //删除导出文件
    return true;
}

bool CPSkypeHelper::AnalyzerSkypeContact(CPRecord *rootRecord)
{
    std::vector<std::wstring> vecAllFiles;
    CPMacFileExport exportFile(SKYPEDIR, SKYPEFILENAME);
    exportFile.GetExportFiles(vecAllFiles);
    std::vector<SkypeContactRecord> vecContactRecords;
    for (std::vector<std::wstring>::iterator it = vecAllFiles.begin();
         it != vecAllFiles.end(); ++it)
    {
        CPSkypeAnalyzer analyzer;
        analyzer.GetSkypeContactInformation(*it, vecContactRecords);
    }
    for (std::vector<SkypeContactRecord>::iterator iter = vecContactRecords.begin();
         iter != vecContactRecords.end(); ++iter )
    {
        CPRecord* Record = rootRecord->addChild();
        Record->at(L"UserName")     = (*iter).m_accountInfo.m_strUserName;
        Record->at(L"DisplayName")  = (*iter).m_accountInfo.m_strDisplayName;
        Record->at(L"SignName")     = (*iter).m_accountInfo.m_strSignName;
        Record->at(L"Country")      = (*iter).m_accountInfo.m_strCountryCode;
        Record->at(L"Province")     = (*iter).m_accountInfo.m_strProvince;
        Record->at(L"City")         = (*iter).m_accountInfo.m_strCity;
        Record->at(L"Sex")          = (*iter).m_accountInfo.m_strSex;
        Record->at(L"Birthday")     = (*iter).m_accountInfo.m_strbirthday;
        Record->at( L"Phone")        = (*iter).m_accountInfo.m_strPhoneNumber;
        Record->at(L"OfficePhone")  = (*iter).m_accountInfo.m_strOfficeNumber;
        Record->at(L"Mobile")       = (*iter).m_accountInfo.m_strMobileNumber;
        Record->at(L"Email")        = (*iter).m_accountInfo.m_strEmail;
        Record->at(L"Website")      = (*iter).m_accountInfo.m_strMainHome;
        Record->at(L"About")        = (*iter).m_accountInfo.m_strAbout;
        Record->at(L"BeLongs")      = (*iter).m_strBelong;
    }
    // 删除导出文件
    return true;
}


bool CPSkypeHelper::AnalyzerSkypeMsgs(CPRecord *rootRecord)
{
    std::vector<std::wstring> vecAllFiles;
    CPMacFileExport exportFile(SKYPEDIR, SKYPEFILENAME);
    exportFile.GetExportFiles(vecAllFiles);
    //GetAllFiles(vecAllFiles, exportFiles, pCtrl);
    std::vector<SkypeMsgRecord> vecMsgRecords;
    for (std::vector<std::wstring>::iterator it = vecAllFiles.begin();
         it != vecAllFiles.end(); ++it)
    {
        CPSkypeAnalyzer analyzer;
        analyzer.GetSkypeMSgInformation(*it, vecMsgRecords);
    }
    for (std::vector<SkypeMsgRecord>::iterator iter = vecMsgRecords.begin();
         iter != vecMsgRecords.end(); ++iter )
    {

        CPRecord* osRecord = rootRecord->addChild();
        osRecord->at(L"UserName") = (*iter).m_strUserName;
        SkypeRecordType msgType = (*iter).m_iMsgType;
        std::wstring strMsgType = L"消息";
        if (msgType == Skype_CallMsg)
        {
            strMsgType = L"语音";
        }
        else if(msgType == Skype_TransferMsg)
        {
            strMsgType = L"文件传输";
        }
        osRecord->at(L"MsgType") = strMsgType;
        osRecord->at(L"Sender") = (*iter).m_strSender;
        osRecord->at(L"Receiver") = (*iter).m_strReceiver;
        osRecord->at(L"Content") = (*iter).m_strContent;
        osRecord->at(L"BeginTime") = (*iter).m_iBeginTime;
        osRecord->at(L"EndTime") = (*iter).m_iEndTime;
        osRecord->at(L"FilePath") = (*iter).m_strFilePath;
        osRecord->at(L"FileName") = (*iter).m_strFileName;
        osRecord->at(L"FileSize") = (*iter).m_strFileSize;
    }
    //删除导出文件
    return true;
}
bool CPSkypeHelper::GetAllFiles(std::vector<std::wstring> &vecAllFiles,
                              std::vector<siutil::CPFileExport*> &exportFiles)
{
    std::wstring strRootPath;
    strRootPath = L"/Users";
    //获取所有分区包含的用户
    std::set<std::wstring> setUsers;
    siutil::MacCommonHelper::GetMacAllUserName(setUsers);

    //遍历获取所有数据文件
    for (std::set<std::wstring>::iterator iterUser = setUsers.begin();
         iterUser != setUsers.end(); ++iterUser)
    {
        std::wstring strAnalysisPath = strRootPath + L"/" + *iterUser + SKYPEDIR;
        iterateAll(exportFiles, strAnalysisPath, vecAllFiles);
    }
    return true;
}

bool CPSkypeHelper::iterateAll(std::vector<siutil::CPFileExport* > &vecExportFiles, const std::wstring &strDir,
                             std::vector<std::wstring> & vecFiles)
{
    std::vector<PSIHANDLE> pathHandles;
    CFileSystem::GetItemHandlesByRawPath(strDir, pathHandles);
  for (std::vector<PSIHANDLE>::iterator iter = pathHandles.begin();
         iter != pathHandles.end(); ++iter)
    {
        //CFileExport pExportFile();
        bool isFileExist = false;
        std::vector<PSIHANDLE> vecHandles;
        PSIHANDLE singleHandle = *iter;
        filesystem::CFileSystem::GetAllSubItems(singleHandle,vecHandles,true,L"");//add
        for (size_t i=0; i<vecHandles.size(); ++i)
        {
            PSIHANDLE subHandle = vecHandles.at(i);
            SPFileItem fileItemInfo;
            CFileSystem::GetFileItemInfo(subHandle, &fileItemInfo);
            if (fileItemInfo.strName == std::wstring(L".") || fileItemInfo.strName == std::wstring(L".."))
            {
                continue;
            }
            else if (fileItemInfo.bFolder)
            {
                std::wstring strAnalysisPath = strDir + L"/" + fileItemInfo.strName;
                iterateAll(vecExportFiles, strAnalysisPath, vecFiles);
            }
            else
            {
                if (wcscmp(fileItemInfo.strName, L"main.db") == 0)
                {
                    //   old                     std::wstring wstrFilePath = pExportFile->Export(subHandle);
                    //   old                     vecFiles.push_back(wstrFilePath);
                    isFileExist = true;
                }
                else if (wcscmp(fileItemInfo.strName, L"main.db-journal") == 0)
                {
                    //      old                  pExportFile->Export(subHandle);
                    isFileExist = true;
                }
                else if (wcscmp(fileItemInfo.strName, L"main.db-shm") == 0)
                {
                    //          old              pExportFile->Export(subHandle);
                    isFileExist = true;
                }
                else if (wcscmp(fileItemInfo.strName, L"main.db-wal") == 0)
                {
                    //                        pExportFile->Export(subHandle);
                    isFileExist = true;
                }

            }
        }
        if (isFileExist)
        {
            //                 vecExportFiles.push_back(pExportFile);
        }
    }
    return true;
}

}

