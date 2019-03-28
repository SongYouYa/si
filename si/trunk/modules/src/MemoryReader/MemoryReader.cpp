#include "MemoryReader.h"
#include "winpmem.h"

/* Create the corrent WinPmem object. Currently this selects between
32/64 bit implementations.
*/
WinPmem *WinPmemFactory() {
	SYSTEM_INFO sys_info;
	ZeroMemory(&sys_info, sizeof(sys_info));

	GetNativeSystemInfo(&sys_info);
	switch (sys_info.wProcessorArchitecture) {
	case PROCESSOR_ARCHITECTURE_AMD64:
		return new WinPmem64();

	case PROCESSOR_ARCHITECTURE_INTEL:
		return new WinPmem32();

	default:
		return NULL;
	}
};

WinPmem *g_pmem_handle = NULL;
struct PmemMemoryInfo* g_memoryInfo = NULL;
unsigned __int64 g_memory_size = 0;

void release_vars()
{
	if (!g_pmem_handle) {
		return;
	}
	g_pmem_handle->uninstall_driver();
	delete g_pmem_handle;
	g_pmem_handle = NULL;

	if (g_memoryInfo) {
		free(g_memoryInfo);
		g_memoryInfo = NULL;
		g_memory_size = 0;
	}
}

int open_memory()
{
	if (g_pmem_handle) {
		//已经打开了
		return 1;
	}
	g_pmem_handle = WinPmemFactory();
	if (!g_pmem_handle) {
		return 1;
	}
	__int64 status = g_pmem_handle->install_driver();
	if (status <= 0) {
		release_vars();
		return 1;
	}
	status = g_pmem_handle->set_acquisition_mode(PMEM_MODE_AUTO);
	if (status <= 0) {
		release_vars();
		return 1;
	}

	DWORD size = 0;
	g_memoryInfo = (PmemMemoryInfo*)malloc(sizeof(PmemMemoryInfo));
	if (!g_memoryInfo) {
		release_vars();
		return 1;
	}
	ZeroMemory(g_memoryInfo, sizeof(PmemMemoryInfo));
	// Get the memory ranges.
	if (!DeviceIoControl(g_pmem_handle->fd_, PMEM_INFO_IOCTRL, 
		NULL, 0, g_memoryInfo,
		sizeof(PmemMemoryInfo), &size, NULL)) {
		release_vars();
		return 1;
	};

	//物理内存大小
	if (g_memoryInfo->NumberOfRuns.QuadPart > 0) {
		__int64 i = g_memoryInfo->NumberOfRuns.QuadPart - 1; //取最后一个区段
		PHYSICAL_MEMORY_RANGE run = g_memoryInfo->Run[i];
		g_memory_size = run.start + run.length;
	}

	return 0;
}

unsigned __int64 memory_size()
{
	return g_memory_size;
}

int read_memory(unsigned __int64 offset, unsigned int length, 
	void* buffer, unsigned int* readed)
{
	if (!g_memoryInfo) {
		return 1;
	}
	if (offset > g_memory_size) {
		return 1;
	}

	char* buf = (char*)buffer;
	PHYSICAL_MEMORY_RANGE* runArray = g_memoryInfo->Run;
	__int64 i = 0;
	__int64 curOffset = offset;
	unsigned int totalReaded = 0;
	unsigned int toRead = 0;
	for (; i < g_memoryInfo->NumberOfRuns.QuadPart; ++i) {
		PHYSICAL_MEMORY_RANGE run = runArray[i];
		if (curOffset >= run.start + run.length) {
			//先找到紧靠近后面的有效区间
			continue;
		}
		if (curOffset < run.start) {
			//在区段之间，填充0
			toRead = (unsigned int)min(run.start - curOffset, length - totalReaded);
			ZeroMemory(buf + totalReaded, toRead);
			totalReaded += toRead;
			curOffset += toRead;
		}
		if (totalReaded >= length) {
			break;
		}
		toRead = (unsigned int)min(run.length, length - totalReaded);
		DWORD apiReaded = 0;
		//读区段的内存数据
		LARGE_INTEGER large_start;
		large_start.QuadPart = curOffset;
		if (0xFFFFFFFF == ::SetFilePointerEx(
			g_pmem_handle->fd_, large_start, NULL, FILE_BEGIN)) {
			return 1;
		};
		BOOL bret = ::ReadFile(g_pmem_handle->fd_, buf + totalReaded, toRead, &apiReaded, NULL);
		if (!bret || apiReaded != toRead) {
			return 1;
		}
		totalReaded += toRead;
		curOffset += toRead;
		if (totalReaded >= length) {
			break;
		}
	}
	if (readed) {
		*readed = totalReaded;
	}
	
	return 0;
}

void close_memory()
{
	release_vars();
}
