#pragma once
#include "application/CommonAnalyzer/globals.h"

namespace app
{
	class CPCallbackFactory
	{
	public:
		static AppCallback getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
			CPTaskStatusCtrl* statusCtrl);
	};
}
