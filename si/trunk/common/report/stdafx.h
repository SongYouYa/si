// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include <vector>
#include <string>
#include <assert.h>
#include <time.h>
#include <iostream>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>


#include "pplatform.h"
#ifdef OS_PWINDOWS
#include "ctemplate/win/ctemplate/template.h"
#elif OS_PUNIX
#include "ctemplate/linux/ctemplate/template.h"
#endif
using namespace ctemplate;

#include "base/globals.h"
#include "base/PTime.h"
#include "base/PRecord.h"
#include "base/PRecordType.h"

#include "report/globals.h"
#include "PLanguageSource.h"
#include "PUtil.h"

#include "sicommon/PCodeChanger.h"
#include "sicommon/PPath.h"

#include "PReportFlags.h"
