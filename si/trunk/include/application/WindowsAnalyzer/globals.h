#pragma once

#ifdef _MSC_VER
#ifdef WIN_APP_EXPORTS
#define WIN_APP_API __declspec(dllexport)
#else
#define WIN_APP_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define WIN_APP_API
#endif