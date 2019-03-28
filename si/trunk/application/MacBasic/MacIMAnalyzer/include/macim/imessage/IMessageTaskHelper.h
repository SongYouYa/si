#ifndef IMESSAGETASKHELPER_H
#define IMESSAGETASKHELPER_H
class CPRecord;
namespace macbasic {

class CPIMessageAnalyzer;

class IMessageTaskHelper
{
public:
    IMessageTaskHelper();
    ~IMessageTaskHelper();

    static bool GetIMessageAccountRecords(CPRecord *rootRecord);
    static bool GetIMessageMsgRecords(CPRecord *rootRecord);

    static bool GetIMessageAccountInfo(CPRecord *rootRecord);
    static bool GetIMessageMsgInfo(CPRecord *rootRecord);


    static bool analysisAccount(const std::wstring & wstrFilePath, CPRecord *rootRecord);
    static bool analysisMsg(const std::wstring & wstrFilePath, CPRecord *rootRecord);

    static bool GetAccountRecord(CPIMessageAnalyzer & reportAnalyzer, CPRecord *rootRecord);
    static bool GetMsgRecord(CPIMessageAnalyzer & reportAnalyzer, CPRecord *rootRecord);
};
}
#endif // IMESSAGETASKHELPER_H
