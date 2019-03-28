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

#include "pplatform.h"
#include "pstd.h"
#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "task/globals.h"
#include "task/PTask.h"
#include "task/PTaskStatusCtrl.h"

#include "sicommon/PString.h"
#include "sicommon/PCodeChanger.h" 
#include "sicommon/PProcess.h"

#include "application/CommonAnalyzer/PAppParam.h"
using namespace app;

#include "siutility/PDirHelper.h"
#include "siutility/PRecordHandler.h"
#include "sisystem/win/POsVersion.h"

#include "PPasswordDefs.h"