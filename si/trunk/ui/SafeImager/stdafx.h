#pragma once

#include <QtWidgets/QtWidgets>
#include <QtCore>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <mutex>

#include <boost/any.hpp>
#include <atomic>
#include <boost/function.hpp>
#include <boost/bind.hpp> 
#include <boost/filesystem.hpp>

#include "pplatform.h"
#include "pstd.h"

#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"

#include "task/globals.h"
#include "task/PTaskCfg.h"
#include "task/PTask.h"
#include "task/PTaskProgress.h"
#include "task/PTaskStatusCtrl.h"

#include "DeviceEnumrator/PDeviceDef.h"
#include "plog/PLog.h"

#include "uicommon/widget/PLabel.h"
using namespace puic;
#include "uicommon/tab/PTabManager.h"
#include "uicommon/button/PToolButton.h"
#include "uicommon/device/PDeviceTreeWidget.h"

typedef std::shared_ptr<QWidget> QWidgetPtr;
typedef std::shared_ptr<QObject> QObjectPtr;

#include "version/version2.h"
#include "version/version_logo.h"

#include "sicommon/PCodeChanger.h"
#include "siutility/PDirHelper.h"
#include "sicommon/PUtils.h"
#include "sicommon/PPath.h"

#include "controller/PTaskController.h"
#include "controller/PCase.h"
#include "sidevice/PDeviceManager.h"

#include "application/DeviceTask/PDeviceTaskParam.h"

#include "logic/PAppContext.h"
#include "ui/common/dialog/PMessageBox.h"
#include "ui/translator/PDynamicTranslator.h"
#include "ui/common/widget/PHSplitLine.h"
#include "handler/PCaseHandler.h"
#include "PTabNameDefs.h"
#include "PReportFlags.h"