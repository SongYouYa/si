// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN  //从 Windows 头文件中排除极少使用的信息
#include <Windows.h>
#endif

#include <vector>
#include <string>
#include <assert.h>
#include <set>

#include "pplatform.h"
#include "pstd.h"

#include "sicommon/PCodeChanger.h"
#include "sicommon/PString.h"

#include "filesystemshell/FileSystemShell.h"
using namespace filesystem;


#include "./encrypt/all_check.h"
#include "./encrypt/CommonCheck.h"
