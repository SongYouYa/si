#ifndef SYSTEMINFO_APPLISTTASKHELPER_H
#define SYSTEMINFO_APPLISTTASKHELPER_H

class CPRecord;
namespace macbasic {

class CAppListAnalyzer;
class CAppListTaskHelper
{
    public:
        CAppListTaskHelper();
        ~CAppListTaskHelper();

        void GetAppListRecords(CPRecord *rootRecord);

    private:
        bool GetAppListInfomation(CPRecord* rootRecord);
        bool GetAppRecord(CAppListAnalyzer & reportAnalyzer, CPRecord* tableRecord);
};

typedef std::map<PSIHANDLE, std::vector<PSIHANDLE> > HistoryAppHandles;

}

#endif // SYSTEMINFO_APPLISTTASKHELPER_
