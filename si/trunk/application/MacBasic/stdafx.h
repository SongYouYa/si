#pragma once

#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>
#include <fstream>
#include <time.h>
#include <sstream>

#include <boost/cstdint.hpp>
#include <boost/signals2.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>

#include "pplatform.h"
#include "pstd.h"
#include "base/globals.h"
#include "base/PTime.h"
#include "base/PVariant.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"
#include "base/PRecordTypeFactory.h"
#include "task/globals.h"
#include "task/PTask.h"
#include "task/PTaskStatusCtrl.h"
#include "sicommon/PString.h"
#include "sicommon/PCodeChanger.h"
#include "filesystemshell/FileSystemShell.h"


#ifdef OS_PLINUX
#include "sqlite3.h"
#include "siutility/PToolDirGeting.h"
#include "siutility/FileExport.h"
#include "siutility/AutoUnitConvert.h"
#include "siutility//FileOperate.h"
#include "siutility/MacCommonHelper.h"
#include "siutility/HashCompute.h"
#include "PlistCpp.h"
#include "siutility/FileSearch.h"
#include "siutility/MacFileExport.h"
#include "filesystemshell/FileSystemShell.h"
//#include "MacMailAnalyzer/include/macmail/MailStruct.h"
#elif OS_PWindows
#include "sisystem\win\POsVersion.h"
#include "sisystem\win\PKnownFolder.h"
#include "TimeConverter.h"
#include "PToolDirGeting.h"
#include "sicommon/PProcess.h"
#include "Win32WMIHelper.h"
#include "OSLocater.h"
#include "siwinregistry/PRegistry.h"

#endif
