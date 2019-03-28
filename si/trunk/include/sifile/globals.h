#pragma once

#ifdef _MSC_VER
#ifdef SIFILE_EXPORTS
#define SIFILE_API __declspec(dllexport)
#else
#define SIFILE_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define SIFILE_API
#endif
