#ifndef PSYSLOGANALLYZER_H
#define PSYSLOGANALLYZER_H

#include "PLogStruct.h"
#include <vector>
namespace macbasic {

class CSysLogAnallyzer
{
    public:
        CSysLogAnallyzer();
        ~CSysLogAnallyzer();

    public:
        bool ParseLogFile(const std::wstring& wstrFile);
        void GetComputerBootInfo(ComputerBootInfos& infos);

    private:
        bool ReadSysLog(std::ifstream * in);
        void StrToTime(std::string str, CPTime & time);

        std::vector<ComputerBootInfo> m_vecSysInfo;
};

}

#endif // PLOGANALLYZER_H
