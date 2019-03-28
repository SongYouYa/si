#pragma once

namespace report
{
	class CPRow;
	class CPCell
	{
	public:
		CPCell(ctemplate::TemplateDictionary* pCellDict, CPRow* pRow);
		~CPCell();

	public:
		CPRow* Row();

		void SetClass(const std::string& strClass);
		void SetRowSpan(int spanCount);
		void SetColSpan(int spanCount);
		int GetColSpan();
		void SetImg(const std::string& strSrc);
		//archorNameΪê�ӵ�Ŀ��name��archorTextΪ��ʾ��
		void SetArchor(const std::string& archorName, const std::string& archorText);

		void SetData(const std::string& strData);
		void SetData(uint64 value);
		void SetData(int64 value);
		void SetData(int value);
		void SetData(uint value);
		void SetData(float f);
		void SetData(double db);

		void SetUrl(const std::string& urlAdd, const std::string& urlDisplay);
		//���ö�ý��ͼƬ
		void SetMediaImg(const std::string& strSrc);
		void SetMediaCommon(const std::string& strPath, const std::string& mediaName);

	private:
		ctemplate::TemplateDictionary* pCellDict_;
		ctemplate::TemplateDictionary* pColSpanDict_;
		int colSpan_;
		CPRow* pRow_;
	};
}