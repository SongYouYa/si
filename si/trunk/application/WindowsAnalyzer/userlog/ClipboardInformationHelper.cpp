#include "stdafx.h"
#include "ClipboardInformationHelper.h"
#include <Psapi.h>
namespace app
{
	ClipboardInformationHelper::ClipboardInformationHelper()
	{
	}
	ClipboardInformationHelper::~ClipboardInformationHelper()
	{
	}
	bool  ClipboardInformationHelper::GetClipboardInformation(ClipboardInformation& clipboard_information)
	{
		if (!OpenClipboard(NULL))	//! 打开剪贴板
			return false;
		std::wstring exe_path; //剪贴板进程全路径
		//! 剪贴板原始的窗口
		HWND hWnd = GetClipboardOwner();
		if (NULL == hWnd)  {
			exe_path = L"";
		}
		else{
			DWORD dwProcessID;
			GetWindowThreadProcessId(hWnd, &dwProcessID);
			exe_path = GetEXEName(dwProcessID);
			clipboard_information.exe_path_ = exe_path.c_str();
		}
		//! 枚举剪贴板数据
		//! 剪贴板格式
		int format = 0;
		while (format = EnumClipboardFormats(format))
		{
			WCHAR format_name[512] = { 0 };
			int name_len = GetClipboardFormatName(format, format_name, 256);
			ClipboardRecord clipboard_record;

			if (0 != name_len || !std::wstring(format_name).empty())	//! 自定义类别
			{
				clipboard_record.format_name_.append(format_name);
			}
	
			else //! 预定义类别
			{
				switch (format)
				{
				case CF_LOCALE:
					clipboard_record.format_name_ = L"CF_LOCALE";
					break;

				case  CF_UNICODETEXT:
					clipboard_record.format_name_ = L"CF_UNICODETEXT";
					break;

				case CF_DIB:
					clipboard_record.format_name_ = L"CF_DIB";
					break;

				case CF_OEMTEXT:
					clipboard_record.format_name_ = L"CF_OEMTEXT";
					break;

				case CF_TEXT:
					clipboard_record.format_name_ = L"CF_TEXT";
					break;
				case CF_BITMAP:
					clipboard_record.format_name_ = L"BITMAP";
					break;

					//case CF_WAVE:
					//	break;
				default:	//! 目前先不考虑对其他类型的支持;
					break;
				}
			}
			std::wstring wstr;
			if (std::wstring(format_name) == L"FileNameW") //文件复制粘贴路径
			{
				HGLOBAL hGlobal = GetClipboardData(format);
				wchar_t* data = (wchar_t *)GlobalLock(hGlobal);
				if (data) {
					wstr = data;
				}
				GlobalUnlock(hGlobal);
			}
			else if (format == CF_TEXT) {
				HGLOBAL hGlobal = GetClipboardData(format);
				char* data = (char *)GlobalLock(hGlobal);
				if (data){
					wstr = CPCodeChanger::GBToUnicode(data);
				}
				GlobalUnlock(hGlobal);
			}
			else if (format == CF_UNICODETEXT){
				HGLOBAL hGlobal = GetClipboardData(format);
				wchar_t* data = (wchar_t *)GlobalLock(hGlobal);
				if (data) {
					wstr = data;
				}
				GlobalUnlock(hGlobal);
			}
			if (!wstr.empty()){
				clipboard_record.clip_data_ = wstr;
				clipboard_information.clipboard_records_.push_back(clipboard_record);
			}
		}
		CloseClipboard();
		return true;
	}
	std::wstring ClipboardInformationHelper::GetEXEName(uint32 dwProcessID)
	{
		DWORD all_process_id[1024];
		uint32 process_count;
		DWORD bytes_count;
		if (!EnumProcesses(all_process_id, sizeof(all_process_id), &bytes_count))
			return L"";
		process_count = bytes_count / sizeof(DWORD);
		WCHAR exe_name[MAX_PATH];
		ZeroMemory(exe_name, MAX_PATH);
		for (uint32 i = 0; i < process_count; i++)
		{
			if (all_process_id[i] == dwProcessID)
			{
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
					PROCESS_VM_READ, FALSE, dwProcessID);

				if (NULL != hProcess)
				{
					HMODULE hMod;
					DWORD bytes_needed;

					if (EnumProcessModules(hProcess, &hMod,
						sizeof(hMod), &bytes_needed))
					{
						GetModuleFileNameEx(hProcess, hMod, exe_name,
							sizeof(exe_name) / sizeof(WCHAR));
						return exe_name;
					}
				}
			}
		}

		return  L"";
	}

	bool ClipboardInformationHelper::analyze(CPRecord* parentRecord)
	{
		ClipboardInformation all_info;
		GetClipboardInformation(all_info);

		std::wstring dataClip =L"";
		for (auto&recordInfo : all_info.clipboard_records_){	
			if (dataClip != recordInfo.clip_data_){
				CPRecord* record = parentRecord->addChild();
				record->at(L"DataSource") = all_info.exe_path_;
				record->at(L"DataContent") = recordInfo.clip_data_;
				record->at(L"DataLength") = recordInfo.clip_data_.length();;
				record->at(L"FormatType") = recordInfo.format_name_;	
			}			
			 dataClip = recordInfo.clip_data_;
		}
	return true;
	}
}
