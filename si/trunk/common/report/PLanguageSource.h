#pragma once

namespace report
{

	enum TEXT_ID
	{
		TEXT_ID_NONE = 0,
		TEXT_ID_NUM,				//���
		TEXT_ID_HTML_HEADER,		//html��header
		TEXT_ID_TYPE,				//����
		TEXT_ID_NAME,				//����
		TEXT_ID_TOTAL_PAGE,			//��ҳ��
		TEXT_ID_PAGE_INDEX,			//ҳ��
		TEXT_ID_STATISTICS,			//����ͳ��
		TEXT_ID_REPORT,				//����
		TEXT_ID_ITEM,				//��Ŀ
		TEXT_ID_CONTENT,			//����
		TEXT_ID_PREVIEW,			//Ԥ��
		TEXT_ID_KEY,				//��
		TEXT_ID_VALUE,				//ֵ

		TEXT_ID_OTHER				//����
	};

	class CPLanguageSource
	{
		CPLanguageSource();
	public:
		~CPLanguageSource();
		static CPLanguageSource* get();

		static void setLang(LANGUAGE language); //Ĭ��Ϊ����
		std::wstring getText(TEXT_ID id);
		
	private:
		static LANGUAGE _language;
		static CPLanguageSource* _instance;

		std::map<TEXT_ID, std::wstring> _TEXT_SRCS;
	};
}

#define GET_TEXT(id) (W2U8(report::CPLanguageSource::get()->getText(id)))
#define GET_WTEXT(id) (report::CPLanguageSource::get()->getText(id))