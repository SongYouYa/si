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

		//获取锚接字符串，构造对象即可获取
		std::string archorName();
		
		void addTitle(const std::string& imgSrc, const std::string& text) override;
		std::string title(); //不包含编号

		//把table添加到某一个css类中
		void AddClass(const std::string& strClass);
		
		//设置head的背景色
		void SetHeadClass(const std::string& strClass);

	protected:
		int Level();
		void SetLevel(int level); //需在设置标题之前调用

	private:
		void AddChild(CPTable* pChild);
		std::string IdStr();

	private:
		std::vector<CPTable*> pChildren_;

		int level_; //层次，用于设置html标题h?
		size_t index_; //同级表格的索引号
		std::string titleStr_;
		std::string idStr_;

		CPTable* pParent_;
		std::string archorName_; //生成的锚接字符串
		
		CPPage* _page; //table所属的html
	};
}
