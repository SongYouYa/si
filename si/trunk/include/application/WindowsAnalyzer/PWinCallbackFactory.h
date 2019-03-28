#pragma once
#include "./globals.h"
#include "application/CommonAnalyzer/globals.h"

namespace app
{
	class WIN_APP_API CPWinCallbackFactory
	{
	public:
		static AppCallback getCallback(PSIHANDLE volume, CPRecord* folderRecord,
			CPTaskStatusCtrl* statusCtrl);
	};
}