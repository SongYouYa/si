#pragma once

namespace tpfile
{
	class CPathRegex;
	class CTemplateApp;
	class CAppRoot;
	class CAppTarget
	{ 
	public:
		CAppTarget();
		~CAppTarget();

		//把此Target下的一级目录或文件加进来
		void AddPathRegex(CPathRegex* target);
		//target 的名称
		void SetName(std::wstring name);
		std::wstring GetName();
		//子节点数目
		size_t PathRegexCount();

		void GetPathRegex(std::vector<CPathRegex*>& regExpPath);
		//设置父AppTemplate节点
		void SetParent(CAppRoot* pAppRoot);
		CAppRoot* GetParent();

		//获取搜索到的所有handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);
	private:
		std::wstring m_Name;                 //Target的名称，类似firefox的cache
		std::vector<CPathRegex*> m_subPathRegex; //target下第一级文件夹或文件
		CAppRoot* m_parent;
	};
}
