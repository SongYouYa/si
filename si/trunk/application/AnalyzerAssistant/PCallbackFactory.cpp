#include "stdafx.h"
#include "PCallbackFactory.h"

#include "application/CommonAnalyzer/PCommonCallbackFactory.h"
#include "application/WindowsAnalyzer/PWinCallbackFactory.h"
#ifdef OS_PUNIX
#include "application/MacAnalyzer/PMacCallbackFactory.h"
#endif

namespace app
{
	const std::wstring WinTablePrefix = L"table.win.";
	const std::wstring MacTablePrefix = L"table.mac.";

	AppCallback CPCallbackFactory::getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
		CPTaskStatusCtrl* statusCtrl)
	{
		if (!folderRecord->isFolder()) {
			return nullptr;
		}
		std::wstring appName = folderRecord->getType()->name();
		if (appName.find(WinTablePrefix) == 0) {
			return CPWinCallbackFactory::getCallback(volume, folderRecord, statusCtrl);
		}
		else if (appName.find(MacTablePrefix) == 0) {
#ifdef OS_PUNIX
            return CPMacCallbackFactory::getAppCallback(volume, folderRecord, statusCtrl);
#endif
		}
		else {
			return CPCommonCallbackFactory::getAppCallback(volume, folderRecord, statusCtrl);
		}
		return nullptr;
	}
}
