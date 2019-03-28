#pragma once

#ifdef _MSC_VER
#ifdef REPORT_EXPORTS
#define REPORT_API __declspec(dllexport)
#else
#define REPORT_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define REPORT_API __attribute__ ((visibility("default")))
#endif

namespace report
{

	enum LANGUAGE
	{
		CHINESE,
		ENGLISH
	};
}
