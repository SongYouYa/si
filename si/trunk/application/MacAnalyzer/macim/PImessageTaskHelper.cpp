#include "stdafx.h"
#include "PImessageTaskHelper.h"
#include "PImessageAnalyzer.h"
#include "siutility/PMacFileExport.h"

using namespace filesystem;
namespace macbasic {
    CPImessageTaskHelper::CPImessageTaskHelper()
    {

    }

    CPImessageTaskHelper::~CPImessageTaskHelper()
    {

    }

    bool CPImessageTaskHelper::getIMessageAccountRecords(CPRecord *rootRecord)
    {

        if (!getIMessageAccountInfo(rootRecord)) {
            return false;
        }
        return true;
    }

    bool CPImessageTaskHelper::getIMessageMsgRecords(CPRecord *rootRecord)
    {    
         if (!getIMessageMsgInfo(rootRecord)) {
            return false;
         }
         return true;
    }

    bool CPImessageTaskHelper::getIMessageAccountInfo(CPRecord *rootRecord)
    {
        std::vector<PSIHANDLE> handles;
        filesystem::CFileSystem::GetItemHandlesByRawPath(L"/Users/*/Library/Preferences/com.Apple.iChat*.plist", handles);

        siutil::CPHandleExport che;
        for (std::vector<PSIHANDLE>::iterator it = handles.begin();
             it != handles.end(); ++it)
        {

            std::wstring filePath = che.Export(*it);
            if(!analysisAccount(filePath,rootRecord))
            {
                continue;
            }
        }
        return true;
    }

    bool CPImessageTaskHelper::getIMessageMsgInfo(CPRecord *rootRecord)
    {
        std::wstring userRawRootPath = L"Library/Messages";
        std::vector<std::wstring> vecFile;
        siutil::CPMacFileExport fileExport(userRawRootPath, L"chat.db");
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

    bool CPImessageTaskHelper::analysisAccount(const std::wstring &wstrFilePath, CPRecord *rootRecord)
    {
        if (wstrFilePath.empty()) {
            return false;
        }
        CPImessageAnalyzer imessageAnalyzer;
        bool bRst = false;
        if (std::wstring::npos == wstrFilePath.rfind(L"com.Apple.iChat.")
                || std::wstring::npos == wstrFilePath.rfind(L".plist")) {
            return false;
        }
        bRst = imessageAnalyzer.addPlist(wstrFilePath);
        if (!bRst) {
            return false;
        }
        return getAccountRecord(imessageAnalyzer, rootRecord);
    }

    bool CPImessageTaskHelper::analysisMsg(const std::wstring &wstrFilePath, CPRecord *rootRecord)
    {
        if (wstrFilePath.empty()) {
            return false;
        }
        CPImessageAnalyzer imessageAnalyzer;
        bool bRst = false;
        if (std::wstring::npos == wstrFilePath.rfind(L"chat.db")) {
            return false;
        }
        bRst = imessageAnalyzer.addMessage(wstrFilePath);
        if (!bRst) {
            return false;
        }
        return getMsgRecord(imessageAnalyzer, rootRecord);
    }

    bool CPImessageTaskHelper::getAccountRecord(CPImessageAnalyzer &reportAnalyzer,CPRecord *rootRecord)
    {
        CPIMAccount * pAccount = reportAnalyzer.FirstChildAccount();
        do {
            if (NULL == pAccount) {
                return false;
            }
             CPRecord* osRecord = rootRecord->addChild();
            osRecord->at(L"SIAccount") = pAccount->m_wstrAccount;
            osRecord->at(L"SIType") = pAccount->m_wstrType;
            osRecord->at(L"SIBelongsSystem") = pAccount->m_wstrBelongsSystem;
        } while ((pAccount = reportAnalyzer.NextChildAccount()));
        return true;
    }

    bool CPImessageTaskHelper::getMsgRecord(CPImessageAnalyzer &reportAnalyzer, CPRecord *rootRecord)
    {
        CPIMMessage * pMessage = reportAnalyzer.FirstChildMsg();
        do {
            if (NULL == pMessage) {
                return false;
            }
           CPRecord* osRecord = rootRecord->addChild();
            osRecord->at(L"SISender") = pMessage->m_wstrSender;
            osRecord->at(L"SIMsgText") = pMessage->m_wstrMsg;
            osRecord->at(L"SITime") = pMessage->m_CPTime;
            osRecord->at(L"SIReceiver") = pMessage->m_wstrReceiver;
        } while((pMessage = reportAnalyzer.NextChildMsg()));
        return true;
    }
}

