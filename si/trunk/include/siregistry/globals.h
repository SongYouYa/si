#pragma once

#ifdef _MSC_VER
#ifdef SIREGISTRY_EXPORTS
#define SIREGISTRY_API __declspec(dllexport)
#else
#define SIREGISTRY_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define SIREGISTRY_API
#endif
