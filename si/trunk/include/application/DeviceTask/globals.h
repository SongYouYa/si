#pragma once

#ifdef _MSC_VER
#ifdef DEV_TASK_EXPORTS
#define DEV_TASK_API __declspec(dllexport)
#else
#define DEV_TASK_API __declspec(dllimport)
#endif
#endif

#ifdef __GNUC__
#define DEV_TASK_API
#endif
