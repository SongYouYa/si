#pragma once

#ifdef _MSC_VER
#ifdef PASS_EXPORTS
#define PASS_API __declspec(dllexport)
#else
#define PASS_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define PASS_API
#endif