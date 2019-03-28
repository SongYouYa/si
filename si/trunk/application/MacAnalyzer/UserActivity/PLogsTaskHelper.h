#ifndef LOGSTASKHELPER_H
#define LOGSTASKHELPER_H

#include "PSysLogAnallyzer.h"
#include "PDailyOutAnalyzer.h"


namespace macbasic {

class LogsTaskHelper
{
    public:
        LogsTaskHelper();
        ~LogsTaskHelper();

        bool GetComputerBootRecord(CPRecord *rootRecord);
        bool GetDailyDiskRecords(CPRecord *rootRecord);
        bool GetDailyNetwRecords(CPRecord *rootRecord);

        bool GetDailyDiskInfo(CPRecord *rootRecord);
        bool GetDailyNetwInfo(CPRecord *rootRecord);

    private:
        void analysisSyslog(const std::wstring & wstrFilePath, CPRecord *rootRecord);
        bool analysisDailyDisk(const std::wstring & wstrFilePath, CPRecord *rootRecord);
        bool analysisDailyNetw(const std::wstring & wstrFilePath,CPRecord *rootRecord);

        bool GetDailyDiskRecord(CPDailyOutAnalyzer & reportAnalyzer,CPRecord *rootRecord);
        bool GetDailyNetwRecord(CPDailyOutAnalyzer & reportAnalyzer,CPRecord *rootRecord);

        static bool TimeCmp(const ComputerBootInfo & c1, const ComputerBootInfo & c2);
};

}

#endif // LOGSTASKHELPER_H
