#pragma once


namespace report
{
	//���б����һ��html�ϣ���������ΪTables
	//���ǵڶ��ֱ�����ʽ�����Լ�2
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
