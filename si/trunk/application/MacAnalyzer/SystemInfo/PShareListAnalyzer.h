#pragma once
namespace macbasic
{
class  CPShareListAnalyzer
{

public:
    typedef  struct
    {
        std::list<std::wstring> wstrSharedAbsoPath;
        std::list<std::wstring> wstrSharedName;
        std::wstring wstrUser;
        std::wstring wstrLimitAuth;
        std::wstring wstrType;
    }ShareList;

    CPShareListAnalyzer();
    ~CPShareListAnalyzer();

    static bool analyze(CPRecord *rootRecord);
    static  bool GetFileShare();
    static  bool GetWebShare();
    static  bool GetBlutoothShare();
    static  bool AnalysisFileShareList(std::wstring filePath);
    static void AnalysisWebShareList(std::wstring webPath);
    static bool AnalysisBlutoothShareList(std::wstring lpath,std::wstring userName);
    static std::pair<std::string, std::string>SplitValue(std::string  strLine, std::string cDelim);
    static void WriteDataInTable(CPRecord* rootRecord,std::vector<ShareList> &vecData);
    static std::wstring SplitUserName(std::wstring longStr);
    static  std::wstring  GetFilePermission(std::wstring FilePath);
private:
    static  std::vector<ShareList> m_FileShared;
};
}

