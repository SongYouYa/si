#ifndef SIBROWSER_COMMON_H
#define SIBROWSER_COMMON_H


#ifdef _WIN32
#ifdef SIBROWSER_EXPORTS
#define SIBROWSER_API __declspec(dllexport)
#else
#define SIBROWSER_API __declspec(dllimport )
#endif
#endif

#ifdef __GNUC__
    #define SIBROWSER_API __attribute__ ((visibility("default")))
#endif

#endif // SIBROWSER_COMMON_H
