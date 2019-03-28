#pragma once

#ifdef _MSC_VER
#ifdef TPFILE_EXPORTS
#define TPFILE_API __declspec(dllexport)
#else
#define TPFILE_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define TPFILE_API
#endif
