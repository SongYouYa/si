#ifndef PARTITIONANALYZER_H
#define PARTITIONANALYZER_H

namespace macbasic {

	class  PartitionAnalyzer
	{
	public:
		PartitionAnalyzer();
		~PartitionAnalyzer();
       static bool  analyze(CPRecord *rootRecord);
       static void GetFileShare();
       static void GetWebShare();
       static void GetBlutoothShare();
       static void AnalysisFileShareList(std::wstring filePath);
       static void AnalysisWebShareList(std::wstring webPath);
       static  std::pair<std::string, std::string> SplitValue( std::string  strLine, std::string cDelim);
       static bool AnalysisBlutoothShareList(std::wstring lpath,std::wstring userName);
	};
}
#endif

