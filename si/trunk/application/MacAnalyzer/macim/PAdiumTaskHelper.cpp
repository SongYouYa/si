#include "stdafx.h"
#include <QDebug>
#include <QString>
#include <qdir.h>
#include <QList>
#include "PAdiumTaskHelper.h"
#include "PAdiumAnalyzer.h"
#include "base/PRecordTypeFactory.h"
#include "siutility/PFileExport.h"

namespace macbasic {
CPAdiumTaskHelper::CPAdiumTaskHelper()
{

}

CPAdiumTaskHelper::~CPAdiumTaskHelper()
{

}

bool CPAdiumTaskHelper::GetAccountInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    //filter informations
    for (std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
         itV != VersionHandles.end(); ++itV) {
          filesystem::CFileSystem::GetAllSubItems(*itV ,handles,true,L"");
    }
    CPHandleExport fileExport;
    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it)
    {
        std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if (std::wstring::npos == filePath.rfind(L"accounts.xml") ||filePath.empty()) {
             continue;
        }
        else{
           std::wstring filePath =  fileExport.Export(*it);
            if(!analysisAccount(filePath,rootRecord)) {
               continue;
            }
        }

    }
    return true;
}

bool CPAdiumTaskHelper::GetContactInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    for(std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
        itV != VersionHandles.end(); ++itV) {
       filesystem::CFileSystem::GetAllSubItems(*itV, handles,false, L"");
    }
    //由于不知道提取总数，默认进度100
    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it)
    {
        std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if (std::wstring::npos == filePath.rfind(L"blist.xml") ||filePath.empty()) {
             continue;
        }
        if(!analysisContact(filePath,rootRecord))
        {
            continue;
        }
    }
    return true;
}

bool CPAdiumTaskHelper::GetMsgInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    for(std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
        itV != VersionHandles.end(); ++itV) {
         filesystem::CFileSystem::GetAllSubItems(*itV,handles, true,L"");
    }
    CPHandleExport fileExport;
    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it){
         std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if (std::wstring::npos == filePath.rfind(L").xml") ||filePath.empty()) {
             continue;
        }else{
            std::wstring xmlfilePath =  fileExport.Export(*it);
          if(!analysisMsg(xmlfilePath,rootRecord)){
              continue;
          }
        }
    }
    return true;
}

bool CPAdiumTaskHelper::analysisAccount(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    CPAdiumAnalyzer adiumAnalyzer;
    bool bRst = false;
    bRst = adiumAnalyzer.AddAccounts(wstrFilePath);
    if (!bRst) {
        return false;
    }

    return GetAccountRecord(adiumAnalyzer, rootRecord);
}

bool CPAdiumTaskHelper::analysisContact(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    CPAdiumAnalyzer adiumAnalyzer;
    bool bRst = false;
    bRst = adiumAnalyzer.AddBlist(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetContactRecord(adiumAnalyzer, rootRecord);
}

bool CPAdiumTaskHelper::analysisMsg(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    CPAdiumAnalyzer adiumAnalyzer; // 每次对文件进行分析是都会创建一个CPAdiumAnalyzer的对象
    bool bRst = false;
    bRst = adiumAnalyzer.AddMessage(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetMsgRecord(adiumAnalyzer, rootRecord);
}

bool CPAdiumTaskHelper::GetAccountRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPAccount *pAccount = reportAnayzer.FirstChildAccount();
    do {
        if (NULL == pAccount) {
            return false;
        }
        CPRecord* osRecord = rootRecord->addChild();
        osRecord->at(L"SIAccount") = pAccount->m_wstrAccount;
        osRecord->at(L"SIAlias") = pAccount->m_wstrAlias;
        osRecord->at(L"SIProtocol") = pAccount->m_adiumProtocol;
    } while((pAccount = reportAnayzer.NextChildAccount()));
    return true;
}

bool CPAdiumTaskHelper::GetContactRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPContact * pContact = reportAnayzer.FirstChildContact();
    do {
        if (NULL == pContact) {
            return false;
        }
        CPRecord* osRecord = rootRecord->addChild();
        osRecord->at(L"SIBelongs") = pContact->m_wstrBelongs;
        osRecord->at(L"SIAccount") = pContact->m_wstrAccount;
        osRecord->at(L"SIAlias") = pContact->m_wstrAlias;
        osRecord->at(L"SIGroupName") = pContact->m_wstrGroupName;
    } while ((pContact = reportAnayzer.NextChildContact()));
    return true;
}

bool CPAdiumTaskHelper::GetMsgRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPMsgRecord * pMsg = reportAnayzer.FirstChildMsg();
    do {
        if (NULL == pMsg) {
            return false;
        }
        CPRecord* osRecord = rootRecord->addChild();
        osRecord->at(L"SISender") = pMsg->m_wstrSender;
        osRecord->at(L"SIReceiver") = pMsg->m_wstrReceiver;
        osRecord->at(L"SIMsgText") = pMsg->m_wstrMsgText;
        osRecord->at(L"SITime") = pMsg->m_wstrTime;
    } while ((pMsg = reportAnayzer.NextChildMsg()));
    return true;
}

void CPAdiumTaskHelper::iteratorVersion(std::vector<PSIHANDLE> & handles)
{
    std::vector<PSIHANDLE> AllHandles;
    std::wstring userRawRootPath = L"Library/Application Support";
    siutil::MacCommonHelper::GetHandlesByUserRawPath(userRawRootPath, L"Adium[^/]*", AllHandles);

    if (AllHandles.empty()) {
        return;
    }

    for (std::vector<PSIHANDLE>::iterator itVersion = AllHandles.begin();
         itVersion != AllHandles.end(); ++itVersion) {
        handles.push_back(*itVersion);
    }
}
}
