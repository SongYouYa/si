#include "stdafx.h"
#include "application/CommonAnalyzer/PCommonCallbackFactory.h"
#include "application/CommonAnalyzer/PDeviceInfoParser.h"

namespace app
{
	AppCallback CPCommonCallbackFactory::getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
		CPTaskStatusCtrl* statusCtrl)
	{
		std::wstring appName = folderRecord->getType()->name();

		//hardware
		if (appName == L"table.diskinfo"){
			return std::bind(CPDeviceInfoParser::getLocalDiskInfos, folderRecord);
		}
		else if (appName == L"table.partitioninfo"){
			return std::bind(CPDeviceInfoParser::getLocalPartitionInfos, folderRecord);
		}
		return nullptr;
	}
}