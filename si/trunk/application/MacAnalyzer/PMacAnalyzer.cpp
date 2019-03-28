#include "stdafx.h"
#include "application/MacAnalyzer/PMacAnalyzer.h"
#include "PReportFlags.h"
#include "base/PRecordTreeCreater.h"

namespace app
{
//������߳�ͬʱ��ȡ�ļ�ϵͳ������ʹ��ʱ�����������ʹ���ļ�ϵͳ��Ӧ�ã�
//Ӧ�û�ȡ��mutex����ֹͬʱ��ȡ�ļ�ϵͳ
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
