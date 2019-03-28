#pragma once

#define MAX_PATH 260

typedef unsigned int DWORD;

#ifndef _FILETIME_
#define _FILETIME_
typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;
#endif

typedef unsigned long long uint64;
typedef long long time64_t;

#define CRITICAL_SECTION pthread_mutex_t

#define WINAPI
typedef void* LPVOID;
#define TRUE 1
typedef int SOCKET;
#define PHOSTENT hostent*
#define SOCKET_ERROR -1

typedef int BOOL;
typedef char CHAR;
typedef unsigned long long ULONGLONG;

#define WSAGetLastError() errno
#define GetTickCount() ((ULONGLONG)time(NULL) * 1000)
#define FAR
#define WSAETIMEDOUT ETIMEDOUT
typedef char TCHAR;
