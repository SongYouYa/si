#include <Windows.h>

//������Ҫ��DLL�е���������Դ����Ҫ֪����DLL�ľ���������ﱣ��һ��
HMODULE g_dllModule = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
	g_dllModule = hModule;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}