#pragma once
namespace app 
{
	class ClipboardInformationHelper
    {

    public:
		struct ClipboardRecord
		{
			std::wstring format_name_;//!剪贴板格式名称
			std::vector<char> data_;
			std::wstring clip_data_;

		};
		typedef std::vector<ClipboardRecord> ClipboardRecords;
		struct ClipboardInformation
		{
			std::wstring exe_path_;       //数据来源 进程路径
			ClipboardRecords clipboard_records_;
			std::wstring strdata;
		};
        ClipboardInformationHelper();
        ~ClipboardInformationHelper();
		static bool analyze(CPRecord* rootRecord);
    private:
	   static bool  GetClipboardInformation(ClipboardInformation& clipboard_information);
       static  std::wstring GetEXEName(uint32 dwProcessID);
    };
}
