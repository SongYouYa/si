#pragma once
namespace app
{
	class  ProcessHandleHelper// : public MemPipeParser 管道信息不足缺少源文件
    {
    public:
		struct HandleInfo
		{
			std::wstring handle_name_;		//句柄名称
			std::wstring hid_;			//句柄ID
			std::wstring	type_;			//句柄类型
			std::wstring share_flags_;
		};
		struct ProcessHandleRecord
		{
			std::wstring process_name_;	//进程名称
			boost::uint32_t pid_;			//进程Id
			std::vector<HandleInfo> handles_;
		};
		typedef std::vector<ProcessHandleRecord> ProcessHandleInformation;
		static bool analyze(CPRecord* rootRecord);
	public:
		static bool  ParseProcessHandleListInformation(ProcessHandleInformation& process_handles_info);

    };
}	
