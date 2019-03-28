#pragma once
namespace app
{
	class  ProcessHandleHelper// : public MemPipeParser �ܵ���Ϣ����ȱ��Դ�ļ�
    {
    public:
		struct HandleInfo
		{
			std::wstring handle_name_;		//�������
			std::wstring hid_;			//���ID
			std::wstring	type_;			//�������
			std::wstring share_flags_;
		};
		struct ProcessHandleRecord
		{
			std::wstring process_name_;	//��������
			boost::uint32_t pid_;			//����Id
			std::vector<HandleInfo> handles_;
		};
		typedef std::vector<ProcessHandleRecord> ProcessHandleInformation;
		static bool analyze(CPRecord* rootRecord);
	public:
		static bool  ParseProcessHandleListInformation(ProcessHandleInformation& process_handles_info);

    };
}	
