#pragma once

#ifdef _WIN32
#ifdef ASSISTANT_EXPORTS
#define ASSISTANT_API __declspec(dllexport)
#else
#define ASSISTANT_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define ASSISTANT_API
#endif
