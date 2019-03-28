#pragma once

#ifdef _MSC_VER
#ifdef SCREEN_LOADER_EXPORTS
#define SCREEN_LOADER_API __declspec(dllexport)
#else
#define SCREEN_LOADER_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define SCREEN_LOADER_API
#endif
