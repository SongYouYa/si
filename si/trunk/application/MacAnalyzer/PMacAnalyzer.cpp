#include "stdafx.h"
#include "application/MacAnalyzer/PMacAnalyzer.h"
#include "PReportFlags.h"
#include "base/PRecordTreeCreater.h"

namespace app
{
//如果多线程同时读取文件系统，反而使总时间更慢，所以使用文件系统的应用，
//应该获取此mutex，防止同时读取文件系统
std::mutex g_macFsMutex;


CPRecord* findRecord(CPRecord *rootRecord, const std::wstring& typeName)
{
    if (rootRecord->getType()->name() == typeName) {
        return rootRecord;
    }
    for(auto child : rootRecord->getChildren()) {
        CPRecord* findedChild = findRecord(child, typeName);
        if (findedChild) {
            return findedChild;
        }
    }
    return nullptr;
}

void CPMacAnalyzer::initRawApps(CPRecord *rootRecord)
{
    CPRecordTreeCreater::get()->createTree(L"mac", rootRecord);
    CPRecord* sysFolderRecord = findRecord(rootRecord, L"table.systemfolder");
    if (sysFolderRecord) {
        sysFolderRecord->setNodeName(L"Mac OS");
        sysFolderRecord->at(L"pvolume") = (PSIHANDLE)nullptr;
    }
    return;
}

void CPMacAnalyzer::initApps(CPRecord *rootRecord)
{
    initRawApps(rootRecord);
}

}
