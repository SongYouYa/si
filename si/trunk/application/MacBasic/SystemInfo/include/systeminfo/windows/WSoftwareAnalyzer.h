#ifndef SYSTEMINFO_WSOFTWAREANALYZER_H
#define SYSTEMINFO_WSOFTWAREANALYZER_H

#include "Common.h"
#include "SystemInfoStruct.h"
#include "Task/TaskDelegate.h"
#include "Task/TaskStatusCtrl.h"

namespace systeminfo
{
	class WSoftwareAnalyzer
	{
	public:
		WSoftwareAnalyzer();
		~WSoftwareAnalyzer();

		void GetSoftwareSystem(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl); //系统软件
		void GetSoftwareApp(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl); //应用软件

	private:
		void ParseSoftwarelist(WAllOsSoftwareInfos& allOsAppInfos);
	};
}
#endif