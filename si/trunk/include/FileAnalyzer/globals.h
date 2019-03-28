#pragma once

#ifdef _MSC_VER
#ifdef FILE_ANA_EXPORTS
#define FILE_ANA_API __declspec(dllexport)
#else
#define FILE_ANA_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define FILE_ANA_API
#endif
