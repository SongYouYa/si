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
			if (GetLastError() == ERROR_MORE_DATA)	//���API���ش��� ˵�����BUF��С����
			{
				//������������Ĵ�����Ϊ����API��������˵��Ҫ�����ֽڵ��ڴ棬����size�����д������API�������ǵĴ�С
				LPENUM_SERVICE_STATUS lpinfo;
				lpinfo = (LPENUM_SERVICE_STATUS)LocalAlloc(LPTR, size);		//����һ��size��С���ڴ�

				if (EnumServicesStatusW(handle, SERVICE_WIN32, SERVICE_STATE_ALL, (LPENUM_SERVICE_STATUSW)lpinfo,
					size, &size, &services_count, NULL))	//�������ȷ�ķ���ȥ����API
				{
					for (uint32 i = 0; i < services_count; i++)
					{
						//�����ServicesCount��API���ص����ˣ���������������������ж��ٸ�Ԫ��
						// ��ǰϵͳ�����״̬
						DWORD dwCurrentState = lpinfo[i].ServiceStatus.dwCurrentState;
						WCHAR* lpStatus = NULL;

						switch (dwCurrentState)
						{
						case SERVICE_CONTINUE_PENDING:
							lpStatus = L"������";
							break;

						case SERVICE_PAUSE_PENDING:
							lpStatus = L"��ͣ��";
							break;

						case SERVICE_PAUSED:
							lpStatus = L"����ͣ";
							break;

						case SERVICE_RUNNING:
							lpStatus = L"������";
							break;

						case SERVICE_START_PENDING:
							lpStatus = L"������";
							break;

						case SERVICE_STOP_PENDING:
							lpStatus = L"ֹͣ��";
							break;

						case SERVICE_STOPPED:
							lpStatus = L"��ֹͣ";
							break;

						default:
							lpStatus = L"δ֪";
						}

						ServiceInformation service_record;
						service_record.service_name_ = std::wstring(lpinfo[i].lpServiceName);
						service_record.display_name_ = std::wstring(lpinfo[i].lpDisplayName);
						service_record.status_ = lpStatus;
						services_information.push_back(service_record);
						//�����lpinfo[i].lpServiceName�Ƿ��������
						//�����lpinfo[i].lpDisplayName�Ƿ���������ʾ������
						//lpinfo[i].ServiceStatus�ṹ���а������Ǹ���ϸ�ķ���״̬��Ϣ��������ο�MSDN
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
