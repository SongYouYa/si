#include "stdafx.h"
#include "MacIMAnalyzer/include/macim/imessage/IMessageTaskHelper.h"
#include "MacIMAnalyzer/include/macim/imessage/PIMessageAnalyzer.h"
using namespace filesystem;
namespace macbasic {
    IMessageTaskHelper::IMessageTaskHelper()
    {

    }

    IMessageTaskHelper::~IMessageTaskHelper()
    {

    }

    bool IMessageTaskHelper::GetIMessageAccountRecords(CPRecord *rootRecord)
    {

        if (!GetIMessageAccountInfo(rootRecord)) {
            return false;
        }

    }

    bool IMessageTaskHelper::GetIMessageMsgRecords(CPRecord *rootRecord)
    {    
         if (!GetIMessageMsgInfo(rootRecord)) {
            return false;
         }
    }

    bool IMessageTaskHelper::GetIMessageAccountInfo(CPRecord *rootRecord)
    {
        std::vector<PSIHANDLE> handles;
        filesystem::CFileSystem::GetItemHandlesByRawPath(L"/Users/*/Library/Preferences/com.Apple.iChat*.plist", handles);

        //由于不知道提取总数，默认进度100
        int i=0;
        siutility::CHandleExport che;
        for (std::vector<PSIHANDLE>::iterator it = handles.begin();
             it != handles.end(); ++it, ++i)
        {

            std::wstring filePath = che.Export(*it);
            if(!analysisAccount(filePath,rootRecord))
            {
                continue;
            }
        }
        return true;
    }

    bool IMessageTaskHelper::GetIMessageMsgInfo(CPRecord *rootRecord)
    {
        std::wstring userRawRootPath = L"Library/Messages";
        std::vector<std::wstring> vecFile;
        siutility::CMacFileExport fileExport(userRawRootPath, L"chat.db");
        fileExport.GetExportFiles(vecFile);

        //由于不知道提取总数，默认进度100

        int i=0;
        for (std::vector<std::wstring>::iterator it = vecFile.begin();
             it != vecFile.end(); ++it, ++i)
        {

            if(!analysisMsg((*it), rootRecord))
            {
                continue;
            }

        }
        return true;
    }

    bool IMessageTaskHelper::analysisAccount(const std::wstring &wstrFilePath, CPRecord *rootRecord)
    {
        if (wstrFilePath.empty()) {
            return false;
        }
        CPIMessageAnalyzer imessageAnalyzer;
        bool bRst = false;
        if (std::wstring::npos == wstrFilePath.rfind(L"com.Apple.iChat.")
                || std::wstring::npos == wstrFilePath.rfind(L".plist")) {
            return false;
        }
        bRst = imessageAnalyzer.AddPlist(wstrFilePath);
        if (!bRst) {
            return false;
        }
        return GetAccountRecord(imessageAnalyzer, rootRecord);
    }

    bool IMessageTaskHelper::analysisMsg(const std::wstring &wstrFilePath, CPRecord *rootRecord)
    {
        if (wstrFilePath.empty()) {
            return false;
        }
        CPIMessageAnalyzer imessageAnalyzer;
        bool bRst = false;
        if (std::wstring::npos == wstrFilePath.rfind(L"chat.db")) {
            return false;
        }
        bRst = imessageAnalyzer.AddMessage(wstrFilePath);
        if (!bRst) {
            return false;
        }
        return GetMsgRecord(imessageAnalyzer, rootRecord);
    }

    bool IMessageTaskHelper::GetAccountRecord(CPIMessageAnalyzer &reportAnalyzer,CPRecord *rootRecord)
    {
        CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.macimessageaccount");
        CPRecord* osRootRecord = osType->newRecord();
        rootRecord->addChild(osRootRecord);
        CPIMAccount * pAccount = reportAnalyzer.FirstChildAccount();
        do {
            if (NULL == pAccount) {
                return false;
            }
             CPRecord* osRecord = osRootRecord->addChild();
            osRecord->at(L"SIAccount") = pAccount->m_wstrAccount;
            osRecord->at(L"SIType") = pAccount->m_wstrType;
            osRecord->at(L"SIBelongsSystem") = pAccount->m_wstrBelongsSystem;
        } while ((pAccount = reportAnalyzer.NextChildAccount()));
        return true;
    }

    bool IMessageTaskHelper::GetMsgRecord(CPIMessageAnalyzer &reportAnalyzer, CPRecord *rootRecord)
    {
        CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.macnormalimmsg");
        CPRecord* osRootRecord = osType->newRecord();
        rootRecord->addChild(osRootRecord);
        CPIMMessage * pMessage = reportAnalyzer.FirstChildMsg();
        do {
            if (NULL == pMessage) {
                return false;
            }
           CPRecord* osRecord = osRootRecord->addChild();
            osRecord->at(L"SISender") = pMessage->m_wstrSender;
            osRecord->at(L"SIMsgText") = pMessage->m_wstrMsg;
            osRecord->at(L"SITime") = pMessage->m_CPTime;
            osRecord->at(L"SIReceiver") = pMessage->m_wstrReceiver;
        } while((pMessage = reportAnalyzer.NextChildMsg()));
        return true;
    }
}

