#pragma once

#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <iostream>
#include <map>
#include <deque>
#include <fstream>
#include <time.h>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
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
#include "sicommon/PString.h"
#include "sicommon/PCodeChanger.h"
#include "filesystemshell/FileSystemShell.h"
using namespace filesystem;
#include "sqlite3.h"
#include "siutility/PDirHelper.h"
#include "siutility/MacCommonHelper.h"
#include "siutility/PFileExport.h"
#include "siutility/PMacFileExport.h"
using namespace siutil;
#include "PlistCpp.h"
#include "siutility/FileSearch.h"
