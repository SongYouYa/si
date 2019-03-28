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

		//�Ѵ�Target�µ�һ��Ŀ¼���ļ��ӽ���
		void AddPathRegex(CPathRegex* target);
		//target ������
		void SetName(std::wstring name);
		std::wstring GetName();
		//�ӽڵ���Ŀ
		size_t PathRegexCount();

		void GetPathRegex(std::vector<CPathRegex*>& regExpPath);
		//���ø�AppTemplate�ڵ�
		void SetParent(CAppRoot* pAppRoot);
		CAppRoot* GetParent();

		//��ȡ������������handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);
	private:
		std::wstring m_Name;                 //Target�����ƣ�����firefox��cache
		std::vector<CPathRegex*> m_subPathRegex; //target�µ�һ���ļ��л��ļ�
		CAppRoot* m_parent;
	};
}
