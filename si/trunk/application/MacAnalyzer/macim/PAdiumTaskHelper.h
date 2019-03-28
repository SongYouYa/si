#ifndef ADIUMTASKHELPER_H
#define ADIUMTASKHELPER_H

namespace macbasic {
class CPAdiumAnalyzer;

class CPAdiumTaskHelper
{
public:
    CPAdiumTaskHelper();
    ~CPAdiumTaskHelper();

    static  bool GetAccountInfo(CPRecord *rootRecord);
    static  bool GetContactInfo(CPRecord *rootRecord);
    static  bool GetMsgInfo(CPRecord *rootRecord);

private:
    static bool analysisAccount(const std::wstring & wstrFilePath, CPRecord *rootRecord);
    static bool analysisContact(const std::wstring & wstrFilePath, CPRecord *rootRecord);
    static bool analysisMsg(const std::wstring & wstrFilePath, CPRecord *rootRecord);

    static bool GetAccountRecord(CPAdiumAnalyzer & reportAnayzer, CPRecord *rootRecord);
    static bool GetContactRecord(CPAdiumAnalyzer & reportAnayzer, CPRecord *rootRecord);
    static bool GetMsgRecord(CPAdiumAnalyzer & reportAnayzer, CPRecord *rootRecord);

    static  void iteratorVersion(std::vector<PSIHANDLE> & handles);
};
}
#endif // ADIUMTASKHELPER_H
