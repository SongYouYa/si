#pragma once

#ifdef _MSC_VER
#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
#include <windows.h>
#endif

#include <string>
#include <map>
#include <vector>
#include <set>

#include "pplatform.h"
#include "pstd.h"
#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecordType.h"

#include "filesystemshell/PInterfaceStruct.h"