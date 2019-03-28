// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <atomic>

#include <boost/filesystem.hpp>


#include "pplatform.h"
#include "pstd.h"

#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"

#include "task/globals.h"
#include "task/PTask.h"
#include "task/PTaskProgress.h"
#include "task/PTaskStatusCtrl.h"

#include "plog/PLog.h"

#include "siio/PNormalFile.h"

#include "libdevtask/PQueue.h"
#include "libdevtask/PMemPool.h"
#include "libdevtask/PDeviceTaskDefs.h"
#include "libdevtask/PHashComputer.h"
#include "libdevtask/PDeviceLogUtils.h"

#include "sidevice/PDeviceManager.h"
#include "sicommon/PPath.h"

#include "application/DeviceTask/PDeviceTaskParam.h"
#include "application/PTaskError.h"
