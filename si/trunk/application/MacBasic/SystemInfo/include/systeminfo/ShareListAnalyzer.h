#ifndef SHARELISTANALYZER_H
#define SHARELISTANALYZER_H

namespace macbasic
{

    class  ShareListAnalyzer
    {

    public:
    typedef struct
    {
        std::list<std::wstring> wstrSharedAbsoPath;
        std::list<std::wstring> wstrSharedName;
        std::wstring wstrUser;
        std::wstring wstrLimitAuth;
        std::wstring wstrType;
    }ShareList;

        ShareListAnalyzer();
        ~ShareListAnalyzer();

         bool  analyze(CPRecord *rootRecord);
        void GetFileShare();
        void GetWebShare();
        void GetBlutoothShare();
        bool AnalysisFileShareList(std::wstring filePath);
        void AnalysisWebShareList(std::wstring webPath);
        bool AnalysisBlutoothShareList(std::wstring lpath,std::wstring userName);
        std::pair<std::string, std::string>SplitValue(std::string  strLine, std::string cDelim);
        void WriteDataInTable(CPRecord* rootRecord,std::vector<ShareList> &vecData);
        std::wstring SplitUserName(std::wstring longStr);
        std::wstring  GetFilePermission(std::wstring FilePath);
private:
       std::vector<ShareList> m_FileShared;
    };
}

#endif
