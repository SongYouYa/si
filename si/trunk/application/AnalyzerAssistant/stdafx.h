// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "pplatform.h"
#include "pstd.h"
#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"
#include "base/PRecordTypeFactory.h"
#include "task/globals.h"
#include "task/PTask.h"
#include "task/PTaskStatusCtrl.h"
#include "task/PTaskProgress.h"

#include "sicommon/PString.h"
#include "sicommon/PCodeChanger.h" 
#include "sicommon/PProcess.h"

#include "filesystemshell/FileSystemShell.h"
using namespace filesystem;

#include "siregistry/PRegistryParser.h"
using namespace reg;

#include "application/CommonAnalyzer/PAppParam.h"
using namespace app;

#ifdef OS_PWINDOWS
#include "sisystem/win/POsVersion.h"
#include "siwinregistry/PRegistry.h"
#endif

#include "siutility/PDirHelper.h"
