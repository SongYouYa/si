#pragma once

//�ɹ�����0�����򷵻ش���0
extern "C" __declspec(dllexport) int open_memory();
//���ڴ�ʧ�ܣ�Ӧ����0
extern "C" __declspec(dllexport) unsigned __int64 memory_size();
//�ɹ�����0�����򷵻ش���0
extern "C" __declspec(dllexport) int read_memory(unsigned __int64 offset, unsigned int length,
	void* buffer, unsigned int* readed);
extern "C" __declspec(dllexport) void close_memory();