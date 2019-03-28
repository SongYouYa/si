#pragma once

namespace report
{
	class CPTree2
	{
		class Impl;
	public:
		CPTree2(CPRecord* rootRecord);
		~CPTree2();

		bool output(const std::wstring& treePath);

	private:
		void getDataString(std::string& data);

		TemplateDictionary* _treeDict;
		CPRecord* _rootRecord;
		Impl* _impl;
	};
}