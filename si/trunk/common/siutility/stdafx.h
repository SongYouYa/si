// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
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
