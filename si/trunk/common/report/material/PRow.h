#pragma once

#include "PCell.h"

namespace report
{
	class CPTableBase;
	class CPRow
	{
	protected:
        CPRow(uint64 index, uint columnCount);
	public:
        virtual ~CPRow();
		uint64 Index();
		virtual CPCell* GetCell(uint column) = 0; //��0��ʼ
		uint ColumnCount();

		void SetTable(CPTableBase* pTable);
		CPTableBase* Table();

	protected:
		std::vector<CPCell*> pCells_;
		CPCell* pNumCell_; //���cell
		uint64 index_; //��0��ʼ
		uint columnCount_;

		static CPCell* CreateCell(ctemplate::TemplateDictionary* pRowDict, CPRow* pRow);
		static ctemplate::TemplateDictionary* CreateRowDict(ctemplate::TemplateDictionary* pTableDict);
	private:
		CPTableBase* pTable_;
	};

	class CPNormalRow : public CPRow
	{
		friend class CPTableBase;
		//��׼���ȷ������, �����������
		CPNormalRow(ctemplate::TemplateDictionary* pRowDict, uint64 index, uint columnCount, bool bNumCol);
	public:
		~CPNormalRow();
		
		CPCell* GetCell(uint column);

	private:
		ctemplate::TemplateDictionary* pRowDict_;
	};

	//�Ǳ�׼���У���Ŀ����������ͬһ�У����ҿ����ж��
	class CPMultiRow : public CPRow
	{
		struct RowInfo
		{
			ctemplate::TemplateDictionary* pRowDict_;
			CPCell* pLastCell_; //�������һ��cell
			uint col_; //���е�ǰ������
			RowInfo() : pRowDict_(NULL), pLastCell_(NULL), col_(0) {}
		};
		friend class CPTableBase;
		//pTableDict:Ϊrow���ڵ�table��dict
		//columnCountΪ�����֮�����������,һ��Ϊż��
		//bool bNumCol : �Ƿ񴴽������
		CPMultiRow(ctemplate::TemplateDictionary* pTableDict, uint64 index, uint columnCount, bool bNumCol);
	public:
//		typedef std::map<std::string, int> Map_Name_Span; //��Ŀ�����п��
		
		~CPMultiRow();

		//ֻ�����ʼ���RowSpanCell
		CPCell* AddRowSpanCell();
		//colSpan Ϊcontent Cell�������
		virtual CPCell* AddCell(const std::string& itemName, int colSpan = 1);
		//�������ӵ�cell������IndexCell������һЩ��cell�Ŀ��
		void Finish();
		virtual CPCell* GetCell(uint column); //��ȡ�ڼ�������cell����0��ʼ

	private:
		RowInfo& GetRowInfo(uint colSpan);

	private:
		ctemplate::TemplateDictionary* pTableDict_;

		ctemplate::TemplateDictionary* pCurRowDict_; //��ǰ��dict
		int rowCount_; //������row����

		std::vector<CPCell*> pRowSpanCells_; //����cell

		std::vector<RowInfo> vecRowInfos_; //�����С��е�ǰ��

		std::vector<CPCell*> pContentCells_; //����cell
		
	};
}
