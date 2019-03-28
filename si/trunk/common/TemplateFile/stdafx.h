// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include <QtCore/QtCore>
#include <QtXml/QtXml>

#include <vector>
#include <string>
#include <assert.h>

#include <boost/thread/mutex.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/shared_ptr.hpp>

#include "pplatform.h"
#include "pstd.h"

#include "sicommon/PCodeChanger.h"
#include "sicommon/PString.h"

#include "filesystemshell/FileSystemShell.h"
using namespace filesystem;
