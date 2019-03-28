#ifndef PDAILYOUTANALYZER_H
#define PDAILYOUTANALYZER_H

#include "PLogStruct.h"
#include <vector>
namespace macbasic {

class CPDailyOutAnalyzer
{
    public:
        CPDailyOutAnalyzer();
        ~CPDailyOutAnalyzer();

        std::vector<DiskStatusInfo> & GetDiskVector();

        std::vector<NetworkInfo> & GetNetwVector();

        bool AddDailyOutFile(const std::wstring & wstrFile);
        bool DailyOutAnalyzer(const std::wstring & wstrFile);

    private:
        bool GetDailyOutTime(const std::string & strLine,struct tm & time);
        bool GetDiskStatusInfo(const std::string & strLine, const CPTime & cTime);
        bool GetNetwStatusInfo(const std::string & strLine, const CPTime & cTime);

        int monthStr2Int(std::string str);

        std::vector<DiskStatusInfo> m_vecDiskInfo;

        std::vector<NetworkInfo> m_vecNetInfo;

        std::vector<std::wstring> m_vecDailyFile;
};

}

#endif // PDAILYOUTANALYZER_H
