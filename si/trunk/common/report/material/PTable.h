#pragma once
#include "./PTableBase.h"


namespace report
{
    class CPPage;
	class CPTable : public CPTableBase
	{
		friend class CPPage;
	protected:
		CPTable(TemplateDictionary* pTableDict, uint columnCount, CPPage* page, CPTable* pParent, size_t rowBeginIndex = 0, bool bNumCol = true);
	public:
		~CPTable();
	public:
		CPPage* page();

		//��ȡê���ַ�����������󼴿ɻ�ȡ
		std::string archorName();
		
		void addTitle(const std::string& imgSrc, const std::string& text) override;
		std::string title(); //���������

		//��table��ӵ�ĳһ��css����
		void AddClass(const std::string& strClass);
		
		//����head�ı���ɫ
		void SetHeadClass(const std::string& strClass);

	protected:
		int Level();
		void SetLevel(int level); //�������ñ���֮ǰ����

	private:
		void AddChild(CPTable* pChild);
		std::string IdStr();

	private:
		std::vector<CPTable*> pChildren_;

		int level_; //��Σ���������html����h?
		size_t index_; //ͬ������������
		std::string titleStr_;
		std::string idStr_;

		CPTable* pParent_;
		std::string archorName_; //���ɵ�ê���ַ���
		
		CPPage* _page; //table������html
	};
}
