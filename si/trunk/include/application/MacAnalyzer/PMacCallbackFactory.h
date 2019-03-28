#pragma once
#include "application/CommonAnalyzer/globals.h"

namespace app
{
    class CPMacCallbackFactory
	{
	public:
        static AppCallback getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
                                   CPTaskStatusCtrl* statusCtrl);
	};
}
