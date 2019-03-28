#include "stdafx.h"
#include "ServiceInfoHelper.h"

namespace app
{
	bool ServiceInfoHelper::GetServicesInformation(ServicesInformation &services_information)
	{
		DWORD services_count = 0;
		DWORD size = 0;
		SC_HANDLE handle = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (handle == NULL)
		{
			return false;
		}

		if (!EnumServicesStatusW(handle, SERVICE_WIN32, SERVICE_STATE_ALL,
			NULL, 0, &size, &services_count, NULL))
		{
			if (GetLastError() == ERROR_MORE_DATA)	//如果API返回错误 说分配的BUF大小不足
			{
				//上面故意引发的错误是为了让API告诉我们说需要多少字节的内存，现在size变量中储存的是API告诉我们的大小
				LPENUM_SERVICE_STATUS lpinfo;
				lpinfo = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, size);		//分配一段size大小的内存

				if (EnumServicesStatusW(handle, SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUSW)lpinfo,
					size, &size, &services_count, NULL))	//这回用正确的方法去请求API
				{
					for (uint32 i = 0; i < services_count; i++)
					{
						//这里的ServicesCount是API返回的咚咚，告诉我们这个数组里面有多少个元素
						// 当前系统服务的状态
						DWORD dwCurrentState = lpinfo[i].ServiceStatus.dwCurrentState;
						WCHAR* lpStatus = NULL;

						switch (dwCurrentState)
						{
						case SERVICE_CONTINUE_PENDING:
							lpStatus = L"继续中";
							break;

						case SERVICE_PAUSE_PENDING:
							lpStatus = L"暂停中";
							break;

						case SERVICE_PAUSED:
							lpStatus = L"已暂停";
							break;

						case SERVICE_RUNNING:
							lpStatus = L"运行中";
							break;

						case SERVICE_START_PENDING:
							lpStatus = L"启动中";
							break;

						case SERVICE_STOP_PENDING:
							lpStatus = L"停止中";
							break;

						case SERVICE_STOPPED:
							lpStatus = L"已停止";
							break;

						default:
							lpStatus = L"未知";
						}

						ServiceInformation service_record;
						service_record.service_name_ = std::wstring(lpinfo[i].lpServiceName);
						service_record.display_name_ = std::wstring(lpinfo[i].lpDisplayName);
						service_record.status_ = lpStatus;
						services_information.push_back(service_record);
						//这里的lpinfo[i].lpServiceName是服务的名称
						//这里的lpinfo[i].lpDisplayName是服务用来显示的名称
						//lpinfo[i].ServiceStatus结构体中包含的是更详细的服务状态信息，具体请参看MSDN
					}
					LocalFree(lpinfo);
				}
			}
		}
		CloseServiceHandle(handle);
		return true;;
	}

	bool ServiceInfoHelper::analyze(CPRecord* parentRecord)
	{
		ServicesInformation  Servicesinfo;
		GetServicesInformation(Servicesinfo);
		if (Servicesinfo.empty()) {
			return false;
		}

		for (auto it = Servicesinfo.begin(); it != Servicesinfo.end(); it++)
		{
			ServiceInformation& Servicesinfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"ServiceName") = Servicesinfo.service_name_;
			record->at(L"DisplayName") = Servicesinfo.display_name_;
			record->at(L"Status") = Servicesinfo.status_;
		}
		return true;
	}
}
