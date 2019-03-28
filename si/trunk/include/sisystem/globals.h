#pragma once

#ifdef _MSC_VER
#ifdef SYSTEM_EXPORTS
#define SYSTEM_API __declspec(dllexport)
#else
#define SYSTEM_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define SYSTEM_API
#endif
