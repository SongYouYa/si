#pragma once

namespace report
{
	class CPTree
	{
		class Impl;
	public:
		CPTree(CPRecord* rootRecord);
		~CPTree();

		bool output(const std::wstring& treePath);

	private:
		void getDataString(std::string& data);

		TemplateDictionary* _treeDict;
		CPRecord* _rootRecord;
		Impl* _impl;
	};
}