#pragma once

#ifdef _MSC_VER
#ifdef FILESYSTEMSHELL_EXPORTS
#define FSS_API __declspec(dllexport)
#else
#define FSS_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define FSS_API
#endif