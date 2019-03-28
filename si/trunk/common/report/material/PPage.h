#pragma once

namespace report
{
	class CPTable;
	class CPPageSet;
	class CPPage
	{
	public:
		CPPage(CPPageSet* pageSet, const std::wstring& filePath);
		~CPPage();
		
		CPPageSet* pageSet();

		void setUpperDir(const std::string& upperDir);
		std::string upperDir();

		CPTable* addTable(uint columnCount, CPTable* pParentTable, size_t rowBeginIndex = 0);

		TemplateDictionary* addPageJumpDict();

		std::wstring filePath();
		//���ʧ�ܣ��򷵻ص�·��Ϊ��
		bool output();

		//�ͷ�
		void release();

	private:
		TemplateDictionary* _pageDict;
		std::vector<CPTable*> _tables;
		std::wstring _filePath;
		CPPageSet* _pageSet;
		std::string _upperDir;
	};
}