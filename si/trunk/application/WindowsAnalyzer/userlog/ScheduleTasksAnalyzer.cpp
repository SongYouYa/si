#include "stdafx.h"
#define _WIN32_DCOM
#include <comdef.h>
//  Include the task header file.
//#include <taskschd.h>
# pragma comment(lib, "taskschd.lib")
# pragma comment(lib, "comsupp.lib")
#include <ATLComTime.h>

#include "ScheduleTasksAnalyzer.h"

namespace app
{


	ScheduleTasksAnalyzer::ScheduleTasksAnalyzer()
	{

	}

	ScheduleTasksAnalyzer::~ScheduleTasksAnalyzer()
	{

	}

	bool ScheduleTasksAnalyzer::Initialize(ITaskFolder*& pRootFolder)
	{
		//  ------------------------------------------------------
		//  Initialize COM.
		HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if( FAILED(hr) )
		{
			printf("\nCoInitializeEx failed: %x", hr );
			return false;
		}

		//  Set general COM security levels.
		hr = CoInitializeSecurity(
			NULL,
			-1,
			NULL,
			NULL,
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
			RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,
			0,
			NULL);

		if( FAILED(hr) )
		{
			DWORD er = ::GetLastError();
			printf("\nCoInitializeSecurity failed: %x", hr );
			if (RPC_E_TOO_LATE != hr) //当结果是RPC_E_TOO_LATE也认为是成功的
			{
				CoUninitialize();
				return false;
			}
		}

		//  ------------------------------------------------------
		//  Create an instance of the Task Service. 
		ITaskService *pService = NULL;
		hr = CoCreateInstance( CLSID_TaskScheduler,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_ITaskService,
			(void**)&pService );  
		if (FAILED(hr))
		{
			printf("Failed to CoCreate an instance of the TaskService class: %x", hr);
			CoUninitialize();
			return false;
		}

		//  Connect to the task service.
		hr = pService->Connect(_variant_t(), _variant_t(),
			_variant_t(), _variant_t());
		if( FAILED(hr) )
		{
			printf("ITaskService::Connect failed: %x", hr );
			pService->Release();
			CoUninitialize();
			return false;
		}

		//  ------------------------------------------------------
		//  Get the pointer to the root task folder.
		//		ITaskFolder *pRootFolder = NULL;
		hr = pService->GetFolder( _bstr_t( L"\\") , &pRootFolder ); //在这里美哟获取道路径信息
		pService->Release();
		if( FAILED(hr) )
		{
			printf("Cannot get Root Folder pointer: %x", hr );
			CoUninitialize();
			return false;
		}

		return true;
	}

	bool ScheduleTasksAnalyzer::ParseFolder(ITaskFolder* pFolder, WinDirTaskScheduleInformation& taskInfos)
	{
		//  -------------------------------------------------------
		//  Get the registered tasks in the folder.
		IRegisteredTaskCollection* pTaskCollection = NULL;
		HRESULT hr = pFolder->GetTasks( TASK_ENUM_HIDDEN, &pTaskCollection );
		if( SUCCEEDED(hr) )
		{
			ParseTaskCollection(pTaskCollection, taskInfos);
		}
		else
		{
			printf("Cannot get the registered tasks.: %x", hr);
		}
		pTaskCollection->Release();

		//parse subFolders
		ITaskFolderCollection* pSubFolderCol = NULL;
		hr = pFolder->GetFolders(0, &pSubFolderCol);
		if (FAILED(hr))
		{
			pFolder->Release();
			return false;
		}
		pFolder->Release();

		long count = 0;
		hr = pSubFolderCol->get_Count(&count);
		if (FAILED(hr))
		{
			pSubFolderCol->Release();
			return false;
		}

		for (long i = 0; i < count; ++i)
		{
			ITaskFolder *pSubFolder = NULL;
			hr = pSubFolderCol->get_Item(_variant_t(i+1), &pSubFolder);
			if (FAILED(hr))
			{
				continue;
			}
#ifdef _DEBUG
			BSTR subFolderName = NULL;
			pSubFolder->get_Name(&subFolderName);
			SysFreeString(subFolderName);
#endif
			WinDirTaskScheduleInformation subTaskInfos;
			ParseFolder(pSubFolder, subTaskInfos);
			taskInfos.insert(taskInfos.end(), subTaskInfos.begin(), subTaskInfos.end());
		}
		pSubFolderCol->Release();

		return true;
	}

	void ScheduleTasksAnalyzer::ParseTaskCollection(IRegisteredTaskCollection* pTaskCollection, WinDirTaskScheduleInformation& taskInfos)
	{
		LONG numTasks = 0;
		HRESULT hr = pTaskCollection->get_Count(&numTasks);

		for(LONG i = 0; i < numTasks; i++)
		{
			IRegisteredTask* pRegisteredTask = NULL;
			hr = pTaskCollection->get_Item( _variant_t(i+1), &pRegisteredTask );
			if( SUCCEEDED(hr) )
			{
				WinDirTaskRecord taskRecord;
				BSTR taskName = NULL;
				hr = pRegisteredTask->get_Name(&taskName);
				if( SUCCEEDED(hr) )
				{
					taskRecord.task_name_ = taskName;
//					printf("\nTask Name: %S", taskName);
					SysFreeString(taskName);
				}
				else
				{
					printf("\nCannot get the registered task name: %x", hr);
				}

				TASK_STATE taskState;
				hr = pRegisteredTask->get_State(&taskState);
				if (SUCCEEDED (hr) )
				{
					taskRecord.status_ = GetTaskStatusStr(taskState);
				}

				DATE date;
				hr = pRegisteredTask->get_NextRunTime(&date);
				if (SUCCEEDED(hr))
				{
					COleDateTime oleDate;
					oleDate.m_dt = date;
					taskRecord.next_run_time_ = CPTime(oleDate.GetYear(), oleDate.GetMonth(), oleDate.GetDay(),
						oleDate.GetHour(), oleDate.GetMinute(), oleDate.GetSecond()).ToString();
				}

				BSTR taskPath = NULL;
				hr = pRegisteredTask->get_Path(&taskPath);
				if (SUCCEEDED(hr))
				{
					std::wstring strpath = taskPath;
					size_t pos = strpath.find_last_of(L'\\');
					if(std::string::npos != pos)
					{
						taskRecord.dir_name_ = strpath.substr(0, pos);
						if (taskRecord.dir_name_.empty())
						{
							taskRecord.dir_name_ = L"\\";
						}
					}
				}
				SysFreeString(taskPath);
				pRegisteredTask->Release();
				taskInfos.push_back(taskRecord);
			}
			else
			{
				printf("\nCannot get the registered task item at index=%d: %x", i+1, hr);
			}
		}

		return;
	}

	std::wstring ScheduleTasksAnalyzer::GetTaskStatusStr( TASK_STATE state )
	{
		std::wstring strState;
		switch(state)
		{
		case TASK_STATE_RUNNING:
			strState = L"正在运行";
			break;
		case TASK_STATE_READY:
			strState = L"就绪";
			break;
		case TASK_STATE_QUEUED:
			strState = L"等待";
			break;
		case TASK_STATE_DISABLED:
			strState = L"未能启动";
			break;
		default:
			strState = L"未知";
			break;
		}
		return strState;
	}

	bool ScheduleTasksAnalyzer::ParseTaskInfo(WinDirTaskScheduleInformation& taskInfos)
	{
		ITaskFolder* pRootFolder = NULL;
		bool ret = Initialize(pRootFolder);
		if (ret)
		{
			ParseFolder(pRootFolder, taskInfos);
		}	
		CoUninitialize();
		return ret;
	}

}