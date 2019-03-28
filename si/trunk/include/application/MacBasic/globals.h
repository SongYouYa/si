#pragma once

#ifdef _MSC_VER
#ifdef APP_BASIC_EXPORTS
#define APP_BASIC_API __declspec(dllexport)
#else
#define APP_BASIC_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define APP_BASIC_API
#endif
