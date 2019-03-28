#ifndef IMESSAGETASKHELPER_H
#define IMESSAGETASKHELPER_H
class CPRecord;
namespace macbasic {

class CPImessageAnalyzer;

class CPImessageTaskHelper
{
public:
    CPImessageTaskHelper();
    ~CPImessageTaskHelper();

    static bool getIMessageAccountRecords(CPRecord *rootRecord);
    static bool getIMessageMsgRecords(CPRecord *rootRecord);

    static bool getIMessageAccountInfo(CPRecord *rootRecord);
    static bool getIMessageMsgInfo(CPRecord *rootRecord);


    static bool analysisAccount(const std::wstring & wstrFilePath, CPRecord *rootRecord);
    static bool analysisMsg(const std::wstring & wstrFilePath, CPRecord *rootRecord);

    static bool getAccountRecord(CPImessageAnalyzer & reportAnalyzer, CPRecord *rootRecord);
    static bool getMsgRecord(CPImessageAnalyzer & reportAnalyzer, CPRecord *rootRecord);
};
}
#endif // IMESSAGETASKHELPER_H
