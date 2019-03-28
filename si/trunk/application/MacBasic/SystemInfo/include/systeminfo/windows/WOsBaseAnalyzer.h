#ifndef SYSTEMINFO_WOSBASEANALYZER_H
#define SYSTEMINFO_WOSBASEANALYZER_H
#include "Common.h"
#include "SystemInfoStruct.h"
#include "Task/TaskDelegate.h"
#include "Task/TaskStatusCtrl.h"

namespace systeminfo
{

	class WOsBaseAnalyzer
	{
	public:
		WOsBaseAnalyzer();
		~WOsBaseAnalyzer();

		void GetOsIdentifyParams(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl);
		void GetOsBaseInfo(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl);

	private:
		void ParseOsBaseInfo(WOsBaseInfos& osInfos);
	};
}


#endif