#include "stdafx.h"
#include "SystemInfo/include/systeminfo/applist/AppListTaskHelper.h"
#include "SystemInfo/include/systeminfo/applist/AppListAnalyzer.h"
#include <QDebug>

using namespace filesystem;

namespace macbasic {

CAppListTaskHelper::CAppListTaskHelper()
{

}

CAppListTaskHelper::~CAppListTaskHelper()
{

}

void CAppListTaskHelper::GetAppListRecords(CPRecord *rootRecord)
{

    CPRecordType* osType = CPRecordTypeFactory::get()->getType(L"table.osidentifyparam");
        CPRecord* osRootRecord = osType->newRecord();
        rootRecord->addChild(osRootRecord);
    if (!GetAppListInfomation(osRootRecord)) {
        return;
    }

}

bool CAppListTaskHelper::GetAppListInfomation(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    CFileSystem::GetItemHandlesByRawPath(L"/Applications/*.app", handles, true);
    CFileSystem::GetItemHandlesByRawPath(L"/Applications/Utilities/*.app", handles, true);

    std::vector<PSIHANDLE> historyPlisthandles;
    CFileSystem::GetItemHandlesByRawPath(L"/Library/Receipts/InstallHistory.plist", historyPlisthandles);

    HistoryAppHandles history_app;
    std::vector<PSIHANDLE> SingleVolumeHandles;

    //区分不同卷的handle
    foreach (PSIHANDLE historyHandle, historyPlisthandles) {
        SingleVolumeHandles.clear();
        foreach (PSIHANDLE appHandle, handles) {
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

    siutility::CHandleExport che;
    int i=0;
    for(HistoryAppHandles::iterator it_m = history_app.begin();
        it_m != history_app.end(); ++it_m)
    {
        PSIHANDLE pHistory = it_m->first;
        std::vector<PSIHANDLE>& appHandles = it_m->second;
        std::wstring installFile = che.Export(pHistory);
       CAppListAnalyzer::ParseInstallHistory(installFile);
        foreach(PSIHANDLE appHandle, appHandles) {
            ++i;
        //    CAppListAnalyzer analyzer;
//            if(analyzer.AddApp(appHandle))
//            {
//                GetAppRecord(analyzer, rootRecord);
//            }
#ifdef _DEBUG
//            else
//            {
//                std::wstring sourcPath = CFileSystem::GetItemPath(appHandle);
//                std::string strpath = CPCodeChanger::UnicodeToUTF8(sourcPath.c_str());
//                std::cout << "source path: " << strpath << "" << std::endl;
//            }
#endif
        }
    }

    return true;
}


bool CAppListTaskHelper::GetAppRecord(CAppListAnalyzer &reportAnalyzer, CPRecord *tableRecord)
{
//    const OSXAPP * pApp = reportAnalyzer.FirstChild();
//    do {
//        if (NULL == pApp) {
//            return false;
//        }
//        CPRecord* appRecord = tableRecord->addChild();
//        appRecord->at(L"Name") = pApp->wstrName;
//        appRecord->at(L"Path") = pApp->wstrPath;
//        appRecord->at(L"Version") = pApp->wstrVersion;
//        appRecord->at(L"otherinfo") = pApp->wstrInfoString;
////        appRecord->at(L"lastModifyTime") = (pApp->modifyTime);
////        appRecord->at(L"StatusChangeTime") = (pApp->lastStatusTime);
////        appRecord->at(L"LastAccessTime") = (pApp->lastReadTime);
////        appRecord->at(L"InstallSource") = pApp->installSource;
//    } while ((pApp = reportAnalyzer.NextChild()));
    return true;
}

}
