#pragma once

namespace tpfile
{
	class CAppTarget; 
	//���ڱ�ʾÿ��Target�µ��ļ���Ŀ¼������·��
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

		//�Ƿ�Ŀ¼/�ļ�
		bool IsDir();
		void SetDir(bool flag);

		//����ƥ��·��
		bool AddRealPath(PSIHANDLE itemHandle);

		//��target 
		void SetRootTarget(CAppTarget* target);
		CAppTarget* GetRootTarget();

		//���ڵ�
		void SetParent(CPathRegex* parent);
		CPathRegex* Parent();

		//��ȡ������������handle
		void GetSearchedHandles(std::vector<PSIHANDLE>& handles);

	private:
		bool m_bDir;         //true :dir; false file
		std::wstring name_; //���õ������ַ���
		//������ʽ����
		boost::xpressive::wsregex m_Reg;

		std::vector<CPathRegex*> m_subRegExpPath; //�����ι�ϵ���Ӽ�
		CAppTarget* m_RootTarget; //��Target
		CPathRegex* m_Parent; //��RegExpPath

		//ƥ�䵽���ļ�
		std::vector<PSIHANDLE> itemHandles_;
	};
}
