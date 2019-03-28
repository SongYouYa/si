#include "stdafx.h"
#include "PLanguageSource.h"

namespace report
{
	LANGUAGE CPLanguageSource::_language = CHINESE;
	CPLanguageSource* CPLanguageSource::_instance = nullptr;


	CPLanguageSource::CPLanguageSource()
	{
		_TEXT_SRCS[TEXT_ID_NONE] = L"";
		_TEXT_SRCS[TEXT_ID_NUM] = L"序号";
		_TEXT_SRCS[TEXT_ID_HTML_HEADER] = L"报告";
		_TEXT_SRCS[TEXT_ID_TOTAL_PAGE] = L"总页数";
		_TEXT_SRCS[TEXT_ID_NAME] = L"名称";
		_TEXT_SRCS[TEXT_ID_PREVIEW] = L"预览";
		_TEXT_SRCS[TEXT_ID_KEY] = L"键";
		_TEXT_SRCS[TEXT_ID_VALUE] = L"值";
	}

	CPLanguageSource::~CPLanguageSource()
	{

	}

	CPLanguageSource* CPLanguageSource::get()
	{
		if (!_instance) {
			_instance = new CPLanguageSource();
		}
		return _instance;
	}

	std::wstring CPLanguageSource::getText(TEXT_ID id)
	{
		auto it = _TEXT_SRCS.find(id);
		if (it == _TEXT_SRCS.end())
		{
			return L"";
		}
		return it->second;
	}

	void CPLanguageSource::setLang(LANGUAGE language)
	{
		_language = language;
	}
}