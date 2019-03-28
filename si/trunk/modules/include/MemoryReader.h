#pragma once

//成功返回0，否则返回大于0
extern "C" __declspec(dllexport) int open_memory();
//打开内存失败，应返回0
extern "C" __declspec(dllexport) unsigned __int64 memory_size();
//成功返回0，否则返回大于0
extern "C" __declspec(dllexport) int read_memory(unsigned __int64 offset, unsigned int length,
	void* buffer, unsigned int* readed);
extern "C" __declspec(dllexport) void close_memory();