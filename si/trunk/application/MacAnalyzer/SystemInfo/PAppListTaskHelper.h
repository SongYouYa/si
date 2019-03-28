#ifndef SYSTEMINFO_APPLISTTASKHELPER_H
#define SYSTEMINFO_APPLISTTASKHELPER_H

namespace macbasic {

class CPAppListAnalyzer;
class CPAppListTaskHelper
{
    public:
        CPAppListTaskHelper();
        ~CPAppListTaskHelper();

       static  bool analyze(CPRecord *rootRecord);

    private:
      static   bool GetAppListInfomation(CPRecord* rootRecord);
       static  bool GetAppRecord(CPAppListAnalyzer & reportAnalyzer, CPRecord* tableRecord);
};

typedef std::map<PSIHANDLE, std::vector<PSIHANDLE> > HistoryAppHandles;

}

#endif // SYSTEMINFO_APPLISTTASKHELPER_
