#pragma once

#ifdef _MSC_VER
#ifdef SI_UTILITY_EXPORTS
#define SI_UTILITY_API __declspec(dllexport)
#else
#define SI_UTILITY_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define SI_UTILITY_API
#endif
