#ifndef PLISTCPP_COMMON_H
#define PLISTCPP_COMMON_H

#ifdef _MSC_VER
#define PLISTCPP_API __declspec(dllexport)
#endif

#ifdef __GNUC__
#define PLISTCPP_API __attribute__ ((visibility("default")))
#endif

#endif