#pragma once

namespace report
{
	class CPPageJumper;
	class CPPage;
	class CPTable;
	class CPPageSet
	{
	public:
		CPPageSet(const std::vector<CPRecord*>& records, const std::wstring& outDir, int level,
			const std::wstring& rootDir, bool withMedia);

		//���ص�һ��html��·��
		bool output();

		//�����Ŀ¼
		std::wstring getRootDir();

	private:

		void addRecords2Table(size_t beginIndex, size_t count, CPTable* pTable);

		std::vector<CPPage*> _pages;

		const std::vector<CPRecord*>& _records;
		std::wstring _outDir;
		int _level;
		std::wstring _rootDir;

		bool _withMedia;
	};
}
