#pragma once


namespace report
{
	//所有表格在一张html上，所以命名为Tables
	//这是第二种报告样式，所以加2
	class CPTable2;
	class CPContent2
	{
	public:
		CPContent2(CPRecord* rootRecord);
		~CPContent2();

		bool output(const std::wstring& filePath);

	private:
		void addTables(CPRecord* parentRecord);

		CPTable2* addRecords2Table(CPRecord* folderRecord, TemplateDictionary* pTableDict);

		CPRecord* _rootRecord;
		TemplateDictionary* _tablesDict;
		std::vector<CPTable2*> _tables;
	};
}
