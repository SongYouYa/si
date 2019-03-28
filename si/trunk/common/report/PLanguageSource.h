#pragma once

namespace report
{

	enum TEXT_ID
	{
		TEXT_ID_NONE = 0,
		TEXT_ID_NUM,				//序号
		TEXT_ID_HTML_HEADER,		//html的header
		TEXT_ID_TYPE,				//类型
		TEXT_ID_NAME,				//名称
		TEXT_ID_TOTAL_PAGE,			//总页数
		TEXT_ID_PAGE_INDEX,			//页数
		TEXT_ID_STATISTICS,			//概述统计
		TEXT_ID_REPORT,				//报告
		TEXT_ID_ITEM,				//项目
		TEXT_ID_CONTENT,			//内容
		TEXT_ID_PREVIEW,			//预览
		TEXT_ID_KEY,				//键
		TEXT_ID_VALUE,				//值

		TEXT_ID_OTHER				//其他
	};

	class CPLanguageSource
	{
		CPLanguageSource();
	public:
		~CPLanguageSource();
		static CPLanguageSource* get();

		static void setLang(LANGUAGE language); //默认为中文
		std::wstring getText(TEXT_ID id);
		
	private:
		static LANGUAGE _language;
		static CPLanguageSource* _instance;

		std::map<TEXT_ID, std::wstring> _TEXT_SRCS;
	};
}

#define GET_TEXT(id) (W2U8(report::CPLanguageSource::get()->getText(id)))
#define GET_WTEXT(id) (report::CPLanguageSource::get()->getText(id))