#include "stdafx.h"
#include "PLogsTaskHelper.h"

namespace macbasic {

LogsTaskHelper::LogsTaskHelper()
{

}

LogsTaskHelper::~LogsTaskHelper()
{

}

bool LogsTaskHelper::GetDailyDiskRecords(CPRecord *rootRecord)
{
    if (!GetDailyDiskInfo(rootRecord)) {
      return false;
    }
    return true;
}

bool LogsTaskHelper::GetDailyNetwRecords(CPRecord *rootRecord)
{ 
    if (!GetDailyNetwInfo(rootRecord)) {
       return false ;
    }
    return true;
}

bool LogsTaskHelper::GetComputerBootRecord(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    CFileSystem::GetItemHandlesByRawPath(L"private/var/log/system.log*", handles);
    
    ComputerBootInfos allRecords;
	size_t i = 0;
     CPHandleExport fileExport;
    for(std::vector<PSIHANDLE>::reverse_iterator it = handles.rbegin(); it != handles.rend(); ++it, ++i) {    
         std::wstring filePath = fileExport.Export(*it);
        ComputerBootInfos oneLogRecord;
        CSysLogAnallyzer analyzer;
        if(analyzer.ParseLogFile(filePath))
        {
            analyzer.GetComputerBootInfo(oneLogRecord);

        }
        allRecords.insert(allRecords.end(), oneLogRecord.begin(), oneLogRecord.end());
    }
    std::sort(allRecords.begin(), allRecords.end(), TimeCmp);
    ComputerBootInfos::iterator pre = allRecords.begin();
    if(pre != allRecords.end())
    {
        pre->AddToTable(rootRecord);
    }
    for(ComputerBootInfos::iterator it = allRecords.begin(); it != allRecords.end(); ++it)
    {
        if(*pre != *it)
        {
            it->AddToTable(rootRecord);
            pre = it;
        }
    }
    return true;
}

bool LogsTaskHelper::GetDailyDiskInfo(CPRecord *table)
{
    std::vector<PSIHANDLE> handles;
    CFileSystem::GetItemHandlesByRawPath(L"private/var/log/daily.out", handles);
    CPHandleExport fileExport;
    for(std::vector<PSIHANDLE>::iterator it = handles.begin();
        it != handles.end(); ++it) {
        std::wstring filePath = fileExport.Export(*it);
        if(!analysisDailyDisk(filePath, table))
        {
            continue;
        }
    }
    return true;
}

bool LogsTaskHelper::GetDailyNetwInfo(CPRecord *rootRecord)
{
    std::vector<PSIHANDLE> handles;
    CFileSystem::GetItemHandlesByRawPath(L"private/var/log/daily.out", handles);
     CPHandleExport fileExport;
    for(std::vector<PSIHANDLE>::iterator it = handles.begin();
        it != handles.end(); ++it) {
         std::wstring filePath = fileExport.Export(*it);
        if(!analysisDailyNetw(filePath, rootRecord))
        {
            continue;
        }
    }
    return true;
}

void LogsTaskHelper::analysisSyslog(const std::wstring &wstrFilePath, CPRecord *table)
{
    CSysLogAnallyzer analyzer;
    if(analyzer.ParseLogFile(wstrFilePath))
    {
        ComputerBootInfos bootInfos;
        analyzer.GetComputerBootInfo(bootInfos);
        for(ComputerBootInfos::iterator it = bootInfos.begin(); it != bootInfos.end(); ++it)
        {
            it->AddToTable(table);
        }
    }
}

bool LogsTaskHelper::analysisDailyDisk(const std::wstring &wstrFilePath, CPRecord *table)
{
    if (wstrFilePath.empty()) {
        return false;
    }
    CPDailyOutAnalyzer analyzer;
    bool bRst = false;
    bRst = analyzer.AddDailyOutFile(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetDailyDiskRecord(analyzer, table);
}

bool LogsTaskHelper::analysisDailyNetw(const std::wstring &wstrFilePath, CPRecord *table)
{
    if (wstrFilePath.empty()) {
        return false;
    }
    CPDailyOutAnalyzer analyzer;
    bool bRst = false;
    bRst = analyzer.AddDailyOutFile(wstrFilePath);
    if (!bRst) {
        return false;
    }
    return GetDailyNetwRecord(analyzer, table);
}

bool LogsTaskHelper::GetDailyDiskRecord(CPDailyOutAnalyzer &reportAnalyzer, CPRecord *table)
{
    std::vector<DiskStatusInfo> vecDisk = reportAnalyzer.GetDiskVector();
    if (vecDisk.empty()) {
        return false;
    }
    for (std::vector<DiskStatusInfo>::iterator it = vecDisk.begin();
         it != vecDisk.end(); ++ it) {
        it->AddToTable(table);
    }
    return true;
}

bool LogsTaskHelper::GetDailyNetwRecord(CPDailyOutAnalyzer &reportAnalyzer, CPRecord *table)
{
    std::vector<NetworkInfo> vecNetw = reportAnalyzer.GetNetwVector();
    if (vecNetw.empty()) {
        return false;
    }
    for (std::vector<NetworkInfo>::iterator it = vecNetw.begin();
         it != vecNetw.end(); ++ it) {
        it->AddToTable(table);
    }
    return true;
}

bool LogsTaskHelper::TimeCmp(const ComputerBootInfo &c1, const ComputerBootInfo &c2)
{
    return c1.logTime >= c2.logTime;
}

}
