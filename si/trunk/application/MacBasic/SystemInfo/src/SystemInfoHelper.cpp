
#include "SystemInfoHelper.h"

#include "PartitionAnalyzer.h"
#include "DeviceInfoAnalyzer.h"

#ifdef SI_MAC
#include "SysBaseInfoAnalyzer.h"
#include "UseInfoAnalyzer.h"
#include "AutorunParser.h"
#include "AppListTaskHelper.h"
#include <boost/bind.hpp>
#include "ShareListAnalyzer.h"
#include "GetComputerName.h"
#else
#include "windows/WOsBaseAnalyzer.h"
#include "windows/WSoftwareAnalyzer.h"
#endif

#ifdef SI_ONLINE_WIN
#include "windows/WUsbDeviceAnalyzer.h"
#endif

namespace systeminfo {

SystemInfoHelper::SystemInfoHelper()
{

}

SystemInfoHelper::~SystemInfoHelper()
{

}


task::CallBack SystemInfoHelper::GetCallbackByTaskId(task::TaskDefId id)
{
	switch(id)
	{
#ifdef SI_MAC
    case task::TaskOsIdentifyParam:
		{
			GetComputerName helper;
			return boost::BOOST_BIND(&GetComputerName::ComputerName,helper,_1,_2);//
		}
	case task::TaskSystemBaseInfo:
		{
			CSysBaseInfoAnalyzer helper;
			return boost::BOOST_BIND(&CSysBaseInfoAnalyzer::GetOperatingBaseInfo,helper,_1,_2);//
		}
	case task::TaskSoftwareInstall:
		{
			AppListTaskHelper appTask;
			return boost::bind(&AppListTaskHelper::GetAppListRecords, appTask, _1, _2);
		}
	case task::TaskAutorun:
		{
			CAutorunParser autoParser;
			return boost::bind(&CAutorunParser::GetAutorunInfo, autoParser, _1, _2);
		}
	case task::TaskUseInfo:
		{
			UseInfoAnalyzer helper;
			return boost::BOOST_BIND(&UseInfoAnalyzer::GetUseInfo,helper,_1,_2);
		}

	case task::TaskShareList:
		{
			ShareListAnalyzer helper;
			return boost::BOOST_BIND(&ShareListAnalyzer::GetShareListInfo,helper,_1,_2);//
		}
	case task::TaskPartitionList:
		{
			PartitionAnalyzer helper;
			return boost::BOOST_BIND(&PartitionAnalyzer::GetPartitionListInfo,helper,_1,_2);//
		}
#else
	case task::TaskOsIdentifyParam:
		{
			WOsBaseAnalyzer baseAna;
			return boost::bind(&WOsBaseAnalyzer::GetOsIdentifyParams, baseAna, _1, _2);
		}
	case task::TaskSystemBaseInfo:
		{
			WOsBaseAnalyzer baseAna;
			return boost::bind(&WOsBaseAnalyzer::GetOsBaseInfo, baseAna, _1, _2);
		}
	case task::TaskSoftwareSystem:
		{
			WSoftwareAnalyzer appAna;
			return boost::bind(&WSoftwareAnalyzer::GetSoftwareSystem, appAna, _1, _2);
		}
	case task::TaskSoftwareApp:
		{
			WSoftwareAnalyzer appAna;
			return boost::bind(&WSoftwareAnalyzer::GetSoftwareApp, appAna, _1, _2);
		}
	case task::TaskDeviceInfo:
		{
			DeviceInfoAnalyzer devAna;
			return boost::bind(&DeviceInfoAnalyzer::GetDeviceInfo, devAna, _1, _2);
		}
	case task::TaskPartitionInfo:
		{
			PartitionAnalyzer partAna;
			return boost::bind(&PartitionAnalyzer::GetPartitionListInfo, partAna, _1, _2);
		}
#endif
#ifdef SI_ONLINE_WIN
    case task::TaskUSBDeviceInfo:
        {
            CWUsbDeviceAnalyzer usbAna;
            return boost::bind(&CWUsbDeviceAnalyzer::GetUsbDeviceInfo, usbAna, _1, _2);
        }
#endif
	}
	return task::CallBack(NULL);
}

std::wstring SystemInfoHelper::GetSystemRootPath()
{
	std::wstring strSystemRootPath;
#ifdef SI_ONLINE_WIN
	WCHAR sysRootPath[MAX_PATH];
	GetWindowsDirectory(sysRootPath, MAX_PATH);
	strSystemRootPath = sysRootPath;
#endif
	return strSystemRootPath;
}


}
