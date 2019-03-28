#include "stdafx.h"
#include "ModulesReliedByProcessHelper.h"

#include <Tlhelp32.h>
#include <Winternl.h>

#include "FileVersionInfo.h"

namespace app
{
	typedef ULONG LOGICAL;
	typedef ULONG *PLOGICAL;
	typedef enum _PROCESSINFOCLASS
	{
		ProcessBasicInformation,
		ProcessQuotaLimits,
		ProcessIoCounters,
		ProcessVmCounters,
		ProcessTimes,
		ProcessBasePriority,
		ProcessRaisePriority,
		ProcessDebugPort,
		ProcessExceptionPort,
		ProcessAccessToken,
		ProcessLdtInformation,
		ProcessLdtSize,
		ProcessDefaultHardErrorMode,
		ProcessIoPortHandlers,          // Note: this is kernel mode only
		ProcessPooledUsageAndLimits,
		ProcessWorkingSetWatch,
		ProcessUserModeIOPL,
		ProcessEnableAlignmentFaultFixup,
		ProcessPriorityClass,
		ProcessWx86Information,
		ProcessHandleCount,
		ProcessAffinityMask,
		ProcessPriorityBoost,
		ProcessDeviceMap,
		ProcessSessionInformation,
		ProcessForegroundInformation,
		ProcessWow64Information,
		MaxProcessInfoClass
	} PROCESSINFOCLASS;
	// begin_ntndis begin_windbgkd
	// NTSTATUS
	typedef LONG NTSTATUS;
	/*lint -save -e624 */  // Don't complain about different typedefs.
	typedef NTSTATUS *PNTSTATUS;
	struct __PEB
	{
		DWORD   dwFiller[4];
		DWORD   dwInfoBlockAddress;
	};
	typedef struct _PEB *PPEB;
	typedef LONG KPRIORITY;
	typedef struct _PROCESS_BASIC_INFORMATION
	{
		NTSTATUS ExitStatus;
		PPEB PebBaseAddress;
		ULONG_PTR AffinityMask;
		KPRIORITY BasePriority;
		ULONG_PTR UniqueProcessId;
		ULONG_PTR InheritedFromUniqueProcessId;
	} PROCESS_BASIC_INFORMATION;
	typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;
	typedef LONG(WINAPI NTQIP)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG);
	struct __INFOBLOCK
	{
		DWORD   dwFiller[16];
		WORD    wLength;
		WORD    wMaxLength;
		DWORD   dwCmdLineAddress;
	};
	/*
	* 获得某个进程的命令行信息
	*
	* 需要自己释放分配的指针
	*/
	WCHAR * ModulesReliedByProcessHelper::GetProceeCmdLine(const uint32 dwProcessID)
	{
		DWORD                       dwSize;
		PROCESS_BASIC_INFORMATION   pbi;
		NTQIP                       *lpfnNtQueryInformationProcess;
		__PEB                       PEB;
		__INFOBLOCK                 Block;
		WCHAR                       *pszCmdLine = NULL;
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessID);

		if (hProcess == NULL)
			return NULL;

		pbi.PebBaseAddress = (_PEB *)0x7ffdf000;
		// we'll default to the above address, but newer OSs might have a different
		// base address for the PEB
		HMODULE hLibrary = GetModuleHandle(L"ntdll.dll");

		if (NULL != hLibrary)
		{
			lpfnNtQueryInformationProcess = (NTQIP *)GetProcAddress(hLibrary, "ZwQueryInformationProcess");

			if (NULL != lpfnNtQueryInformationProcess)
				(*lpfnNtQueryInformationProcess)(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);
		}

		BOOL bSuccess = ReadProcessMemory(hProcess, pbi.PebBaseAddress, &PEB, sizeof(PEB), &dwSize);

		if (bSuccess != FALSE)
		{
			bSuccess = ReadProcessMemory(hProcess, (LPVOID)PEB.dwInfoBlockAddress, &Block, sizeof(Block), &dwSize);
			pszCmdLine = new WCHAR[Block.wMaxLength];
		}

		if (bSuccess != FALSE)
			bSuccess = ReadProcessMemory(hProcess, (LPVOID)Block.dwCmdLineAddress, pszCmdLine, Block.wMaxLength, &dwSize);
		else
		{
			delete[]pszCmdLine;
			pszCmdLine = NULL;
		}

		return pszCmdLine;
	}
	ModulesReliedByProcessHelper::ModulesReliedByProcessHelper()
	{
	}
	ModulesReliedByProcessHelper::~ModulesReliedByProcessHelper()
	{
	}
	bool ModulesReliedByProcessHelper::GetAllModules(AllModulesInformation& all_modules_information)
	{
		HANDLE snap_process = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe;                                 //定义一个PROCESSENTRY32结构
		pe.dwSize = sizeof(pe);       //初始化
		BOOL next = Process32First(snap_process, &pe);
		while (next){
			ModulesReliedByOneProcessInformation modules_process_info;
			modules_process_info.process_id_ = pe.th32ProcessID;
			modules_process_info.process_name_ = pe.szExeFile;
			std::wstring tmp = L"";

			if (GetProceeCmdLine(pe.th32ProcessID) == NULL){
				tmp = L"无法获取命令行";
			}
			else{
				tmp = GetProceeCmdLine(pe.th32ProcessID);
			}
			modules_process_info.cmd_line_ = tmp.c_str();
			GetModulesReliedByProcessInformation(pe.th32ProcessID, modules_process_info.modules_info_);
			all_modules_information.push_back(modules_process_info);
			next = Process32Next(snap_process, &pe);
		}
		CloseHandle(snap_process);
		return true;
	}
	bool ModulesReliedByProcessHelper::GetModulesReliedByProcessInformation(const uint32 process_id,
		ModulesInformation& modules_process_information)
	{
		HANDLE snap_module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
		MODULEENTRY32 me;
		me.dwSize = sizeof(me);
		BOOL next_module = Module32First(snap_module, &me);
		WCHAR buffer[256];

		while (next_module)
		{
			ModuleInformation module_info;
			//(buffer, L"0x%08X", me.modBaseAddr);
			module_info.base_addr_ = buffer;
			ZeroMemory(buffer, sizeof(buffer));
			swprintf(buffer, 100, L"0x%08X", me.modBaseSize);
			module_info.base_size_ = buffer;
			CFileVersionInfo file_version_info;
			if (file_version_info.Create(me.szExePath))	{
				WCHAR file_version[MAX_PATH];
				swprintf(file_version, 100, L"%d.%2d.%4d.%4d",
					file_version_info.GetFileVersion(3), file_version_info.GetFileVersion(2),
					file_version_info.GetFileVersion(1), file_version_info.GetFileVersion(0));
				module_info.version_ = std::wstring(file_version);
			}
			else	{
				module_info.version_ = L"";
			}
			module_info.exe_path_ = me.szExePath;
			modules_process_information.push_back(module_info);
			next_module = Module32Next(snap_module, &me);
		}
		CloseHandle(snap_module);
		return true;
	}

	bool ModulesReliedByProcessHelper::analyze(CPRecord* parentRecord)
	{
		AllModulesInformation  Modulesinfo;
		GetAllModules(Modulesinfo);
		if (Modulesinfo.empty()) {
			return false;
		}

		for (auto it = Modulesinfo.begin(); it != Modulesinfo.end(); it++){
			ModulesReliedByOneProcessInformation& Modulesinfo = *it;
			CPRecord* record = parentRecord->addChild();
			record->at(L"ProcessID") = Modulesinfo.process_id_;
			record->at(L"ProcessName") = Modulesinfo.process_name_;
			record->at(L"Commandline") = Modulesinfo.cmd_line_;
			for (auto &modelInfo : Modulesinfo.modules_info_){
				record->at(L"BaseAddress") = modelInfo.base_addr_;
				record->at(L"ModuleSize") = modelInfo.base_size_;
				record->at(L"Version") = modelInfo.version_;
				record->at(L"Path") = modelInfo.exe_path_;
			}
		}
		return true;
	}
}