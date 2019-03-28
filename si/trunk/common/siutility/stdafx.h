// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#ifdef OS_PWINDOWS
#include <windows.h>
#endif
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
#include <atomic>
#include <fstream>
#include <memory>
#include <mutex>

#include <boost/any.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include "pplatform.h"
#include "pstd.h"

#include "sicommon/PUtils.h"
#include "sicommon/PCodeChanger.h"

#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"

#include "DeviceEnumrator/PDeviceDef.h"
