#pragma once
#include "./globals.h"

namespace app
{
	class CPCommonCallbackFactory
	{
	public:
		static AppCallback getAppCallback(PSIHANDLE volume, CPRecord* folderRecord,
			CPTaskStatusCtrl* statusCtrl);
	};
}
