#pragma once

namespace tpfile
{
	class CAppTarget; 
	//用于表示每个Target下的文件或目录的正则路径
	class CPathRegex
	{
	public:
		CPathRegex();
		~CPathRegex();

		void SetName(const std::wstring& name);
		std::wstring GetName();

		bool IsMatch(const std::wstring& name);

		void AddSubPathRegex(CPathRegex* subPath);
		void GetSubPathRegex(std::vector<CPathRegex*>& vSubPath);
		size_t SubPathRegexCount();

		//是否目录/文件
		bool IsDir();
		void SetDir(bool flag);

		//保存匹配路径
		bool AddRealPath(PSIHANDLE itemHandle);

		//父target 
		void SetRootTarget(CAppTarget* target);
		CAppTarget* GetRootTarget();

		//父节点
		void SetParent(CPathRegex* parent);
		CPathRegex* Parent();

		//获取搜索到的所有handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);

	private:
		bool m_bDir;         //true :dir; false file
		std::wstring name_; //配置的名字字符串
		//正则表达式对象
		boost::xpressive::wsregex m_Reg;

		std::vector<CPathRegex*> m_subRegExpPath; //保存层次关系的子集
		CAppTarget* m_RootTarget; //父Target
		CPathRegex* m_Parent; //父RegExpPath

		//匹配到的文件
		std::vector<PSIHANDLE> itemHandles_;
	};
}
