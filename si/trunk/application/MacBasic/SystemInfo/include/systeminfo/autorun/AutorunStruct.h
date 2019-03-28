#ifndef SYSTEMINFO_AUTORUNSTRUCT_H
#define SYSTEMINFO_AUTORUNSTRUCT_H

#define ALL_USER_STR L"所有用户"
class CPRecord;
struct AutoRunInfo
{
    std::wstring type_;
    std::wstring fileName_; //
    std::wstring command_;
    std::wstring user_;
    std::wstring description_;
    std::wstring filePath_;
    CPTime lastAccessTime_;
    CPTime lastModifyTime_;
    std::wstring remark_;

    void AddToTable(CPRecord* table)
    {
        CPRecord* fileRecord = table->addChild();
        fileRecord->at(L"Type") = type_;
        fileRecord->at(L"FileName") = fileName_;
        fileRecord->at(L"Command") = command_;
        fileRecord->at(L"User") = user_;
        fileRecord->at(L"Description") = description_;
        fileRecord->at(L"Path") = filePath_;
        fileRecord->at(L"LastAccess") = lastAccessTime_;
        fileRecord->at(L"LastModify") = lastModifyTime_;
        fileRecord->at(L"Remark") = remark_;
    }
};
typedef std::vector<AutoRunInfo> AutoRunInfos;

#endif // SYSTEMINFO_AUTORUNSTRUCT_H
