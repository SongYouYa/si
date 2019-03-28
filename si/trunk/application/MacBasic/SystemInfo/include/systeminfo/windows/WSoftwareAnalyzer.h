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

		void GetSoftwareSystem(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl); //ϵͳ���
		void GetSoftwareApp(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl); //Ӧ�����

	private:
		void ParseSoftwarelist(WAllOsSoftwareInfos& allOsAppInfos);
	};
}
#endif