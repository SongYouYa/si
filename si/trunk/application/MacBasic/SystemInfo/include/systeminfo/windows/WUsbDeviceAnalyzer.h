#ifndef SI_SYSTEMXX_USBDEVICEINFOHELPER_H
#define SI_SYSTEMXX_USBDEVICEINFOHELPER_H

#include "Common.h"
#include "Task/TaskDelegate.h"
#include "Task/TaskStatusCtrl.h"

namespace systeminfo
{
	class CWUsbDeviceAnalyzer
	{
	public:
		CWUsbDeviceAnalyzer();
		~CWUsbDeviceAnalyzer();

		void GetUsbDeviceInfo(task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl);

	private:
		//��ȡnirsoft��cmd�����в���
		std::wstring GetNirsoftCmdstr();
		std::wstring GetOutputXmlPath();

		bool ParseUsbDevice(std::wstring xmlPath, task::TaskDelegate* pDelegate, task::TaskStatusCtrl* pCtrl);
	};
}

#endif