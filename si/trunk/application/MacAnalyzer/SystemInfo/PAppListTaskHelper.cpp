#include "stdafx.h"
#include "PAppListTaskHelper.h"
#include "PAppListAnalyzer.h"

namespace macbasic {

CPAppListTaskHelper::CPAppListTaskHelper()
{

}

CPAppListTaskHelper::~CPAppListTaskHelper()
{

}

bool CPAppListTaskHelper::analyze(CPRecord *rootRecord)
{
    if (!GetAppListInfomation(rootRecord)) {
        return false;
    }
    return true;
}

bool CPAppListTaskHelper::GetAppListInfomation(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    CFileSystem::GetItemHandlesByRawPath(L"/Applications/*.app", handles, true);
    CFileSystem::GetItemHandlesByRawPath(L"/Applications/Utilities/*.app", handles, true);

    std::vector<PSIHANDLE> historyPlisthandles;
    CFileSystem::GetItemHandlesByRawPath(L"/Library/Receipts/InstallHistory.plist", historyPlisthandles);

    HistoryAppHandles history_app;
    std::vector<PSIHANDLE> SingleVolumeHandles;

    //区分不同卷的handle
    for (PSIHANDLE historyHandle :  historyPlisthandles) {
        SingleVolumeHandles.clear();
        for (PSIHANDLE appHandle : handles) {
            if (CFileSystem::InSameVolume(historyHandle, appHandle)) {
                SingleVolumeHandles.push_back(appHandle);
            }
        }
        history_app[historyHandle] = SingleVolumeHandles;
    }

#ifdef _DEBUG
    std::cout << "app:historyhandles count: " << historyPlisthandles.size() << "" << std::endl;
    std::cout << "app:handle count: " << handles.size() << "" << std::endl;
#endif

    int i=0;
    CPHandleExport fileExport;
    for(HistoryAppHandles::iterator it_m = history_app.begin();
        it_m != history_app.end(); ++it_m)
    {
        PSIHANDLE pHistory = it_m->first;
        std::vector<PSIHANDLE>& appHandles = it_m->second;
        std::wstring installFile = fileExport.Export(pHistory);
        CPAppListAnalyzer::ParseInstallHistory(installFile);
        for(PSIHANDLE appHandle : appHandles) {
            ++i;
            CPAppListAnalyzer analyzer;
            if(analyzer.AddApp(appHandle))
            {
                GetAppRecord(analyzer, rootRecord);
            }
#ifdef _DEBUG
            else
            {
                std::wstring sourcPath = CFileSystem::GetItemPath(appHandle);
                std::string strpath = CPCodeChanger::UnicodeToUTF8(sourcPath.c_str());
                std::cout << "source path: " << strpath << "" << std::endl;
            }
#endif
        }
    }

    return true;
}


bool CPAppListTaskHelper::GetAppRecord(CPAppListAnalyzer &reportAnalyzer, CPRecord *tableRecord)
{
    const OSXApp * pApp = reportAnalyzer.FirstChild();
    do {
        if (NULL == pApp) {
            return false;
        }
        CPRecord* appRecord = tableRecord->addChild();
        appRecord->at(L"Name") = pApp->wstrName;
        appRecord->at(L"Path") = pApp->wstrPath;
        appRecord->at(L"Version") = pApp->wstrVersion;
        appRecord->at(L"otherinfo") = pApp->wstrInfoString;
        appRecord->at(L"lastModifyTime") = (pApp->modifyTime);
        appRecord->at(L"StatusChangeTime") = (pApp->lastStatusTime);
        appRecord->at(L"LastAccessTime") = (pApp->lastReadTime);
        appRecord->at(L"InstallSource") = pApp->installSource;
    } while ((pApp = reportAnalyzer.NextChild()));
    return true;
}

}
