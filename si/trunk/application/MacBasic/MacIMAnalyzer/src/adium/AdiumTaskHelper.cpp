#include "stdafx.h"
#include <QDebug>
#include <QString>
#include <qdir.h>
#include <QList>
#include "MacIMAnalyzer/include/macim/adium/AdiumTaskHelper.h"
#include "MacIMAnalyzer/include/macim/adium/PAdiumAnalyzer.h"
#include "base/PRecordTypeFactory.h"
using namespace filesystem;
namespace macbasic {
AdiumTaskHelper::AdiumTaskHelper()
{

}

AdiumTaskHelper::~AdiumTaskHelper()
{

}

bool AdiumTaskHelper::GetAccountInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    //filter informations
    for (std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
         itV != VersionHandles.end(); ++itV) {
         CFileSystem::GetAllSubItems(*itV ,false,L"",&handles);
    }

    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it, ++it)
    {
        std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if(!analysisAccount(filePath,rootRecord)) {
            continue;
        }
    }
    return true;
}

bool AdiumTaskHelper::GetContactInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    for(std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
        itV != VersionHandles.end(); ++itV) {
        CFileSystem::GetAllSubItems(*itV, false, L"", &handles);
    }
    //由于不知道提取总数，默认进度100
    int i=1;
    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it, ++i)
    {
        std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if(!analysisContact(filePath,rootRecord))
        {
            continue;
        }
    }
    return true;
}

bool AdiumTaskHelper::GetMsgInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    std::vector<PSIHANDLE> VersionHandles;
    iteratorVersion(VersionHandles);
    for(std::vector<PSIHANDLE>::iterator itV = VersionHandles.begin();
        itV != VersionHandles.end(); ++itV) {
        CFileSystem::GetAllSubItems(*itV, true,L"",&handles);
    }
    //由于不知道提取总数，默认进度100
    int i=1;
    for (std::vector<PSIHANDLE>::iterator it = handles.begin();
         it != handles.end(); ++it, ++i)
    {
        std::wstring filePath = filesystem::CFileSystem::GetItemPath(*it);
        if(!analysisMsg(filePath,rootRecord))
        {
            continue;
        }
    }
    return true;
}

bool AdiumTaskHelper::analysisAccount(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    if (wstrFilePath.empty()) {
        return false;
    }
    CPAdiumAnalyzer adiumAnalyzer;
    bool bRst = false;
    if (std::wstring::npos == wstrFilePath.rfind(L"accounts.xml")) {
        return false;
    }
    bRst = adiumAnalyzer.AddAccounts(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetAccountRecord(adiumAnalyzer, rootRecord);
}

bool AdiumTaskHelper::analysisContact(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    if (wstrFilePath.empty()) {
        return false;
    }
    CPAdiumAnalyzer adiumAnalyzer;
    bool bRst = false;
    if (std::wstring::npos == wstrFilePath.rfind(L"blist.xml")) {
        return false;
    }
    bRst = adiumAnalyzer.AddBlist(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetContactRecord(adiumAnalyzer, rootRecord);
}

bool AdiumTaskHelper::analysisMsg(const std::wstring &wstrFilePath, CPRecord *rootRecord)
{
    if (wstrFilePath.empty()) {
        return false;
    }
    CPAdiumAnalyzer adiumAnalyzer; // 每次对文件进行分析是都会创建一个CPAdiumAnalyzer的对象
    bool bRst = false;
    if (std::wstring::npos == wstrFilePath.rfind(L").xml")) {
        return false;
    }
    bRst = adiumAnalyzer.AddMessage(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetMsgRecord(adiumAnalyzer, rootRecord);
}

bool AdiumTaskHelper::GetAccountRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.macadiumaccoun");
    CPRecord* osRootRecord = osType->newRecord();
    rootRecord->addChild(osRootRecord);
    CPAccount *pAccount = reportAnayzer.FirstChildAccount();
    do {
        if (NULL == pAccount) {
            return false;
        }
        CPRecord* osRecord = osRootRecord->addChild();
        osRecord->at(L"SIAccount") = pAccount->m_wstrAccount;
        osRecord->at(L"SIAlias") = pAccount->m_wstrAlias;
        osRecord->at(L"SIProtocol") = pAccount->m_adiumProtocol;
    } while((pAccount = reportAnayzer.NextChildAccount()));
    return true;
}

bool AdiumTaskHelper::GetContactRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.macadiumcontact");
    CPRecord* osRootRecord = osType->newRecord();
    rootRecord->addChild(osRootRecord);
    CPContact * pContact = reportAnayzer.FirstChildContact();
    do {
        if (NULL == pContact) {
            return false;
        }
        CPRecord* osRecord = osRootRecord->addChild();
        osRecord->at(L"SIBelongs") = pContact->m_wstrBelongs;
        osRecord->at(L"SIAccount") = pContact->m_wstrAccount;
        osRecord->at(L"SIAlias") = pContact->m_wstrAlias;
        osRecord->at(L"SIGroupName") = pContact->m_wstrGroupName;
    } while ((pContact = reportAnayzer.NextChildContact()));
    return true;
}

bool AdiumTaskHelper::GetMsgRecord(CPAdiumAnalyzer &reportAnayzer, CPRecord *rootRecord)
{
    CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.macnormalimmsg");
    CPRecord* osRootRecord = osType->newRecord();
    rootRecord->addChild(osRootRecord);
    CPMsgRecord * pMsg = reportAnayzer.FirstChildMsg();
    do {
        if (NULL == pMsg) {
            return false;
        }
        CPRecord* osRecord = osRootRecord->addChild();
        osRecord->at(L"SISender") = pMsg->m_wstrSender;
        osRecord->at(L"SIReceiver") = pMsg->m_wstrReceiver;
        osRecord->at(L"SIMsgText") = pMsg->m_wstrMsgText;
        osRecord->at(L"SITime") = pMsg->m_wstrTime;
    } while ((pMsg = reportAnayzer.NextChildMsg()));
    return true;
}

void AdiumTaskHelper::iteratorVersion(std::vector<PSIHANDLE> & handles)
{
    std::vector<PSIHANDLE> AllHandles;
    std::wstring userRawRootPath = L"Library/Application Support";
    siutility::MacCommonHelper::GetHandlesByUserRawPath(userRawRootPath, L"Adium[^/]*", AllHandles);

    if (AllHandles.empty()) {
        return;
    }

    for (std::vector<PSIHANDLE>::iterator itVersion = AllHandles.begin();
         itVersion != AllHandles.end(); ++itVersion) {
        handles.push_back(*itVersion);
    }
}
}
