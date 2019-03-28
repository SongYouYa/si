#pragma once

namespace app
{
    class  ProcessInfoHelper
    {
		struct ProcessRecord
		{
			uint32 process_id_;
			std::wstring process_name_;
			uint32 thread_count_;
			std::wstring path_;
			uint32  handleCount_;
			std::wstring userName_;
			uint32 session_id_;
			uint32  userObject_;
			uint64_t memory_workingset_size_;
			std::wstring  description_;
		};
		typedef std::vector<ProcessRecord> ProcessInformation;

    public:
		static bool analyze(CPRecord* parentRecord);
       
	private:
		static bool  GetProcessListInformation(ProcessInformation& process_info);
		static LPCTSTR GetProcessUserName( HANDLE hProcess);
		static std::wstring GetFileVersion(const std::wstring& strFilePath);
		static std::wstring DosDevicePath2LogicalPath(LPCTSTR lpszDosPath);
    };
}
