
#ifndef _MAC_SYSTEMINFO_COMMON_H
#define _MAC_SYSTEMINFO_COMMON_H
#ifdef WIN32
#ifdef SYSTEMINFO_EXPORTS
#define SYSTEMINFO_API __declspec(dllexport)
#else
#define SYSTEMINFO_API __declspec(dllimport )
#endif
#else
#define SYSTEMINFO_API __attribute__ ((visibility("default")))
#endif
#include "common/common.h"
#endif // COMMON_H
