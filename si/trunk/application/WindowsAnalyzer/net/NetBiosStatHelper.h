#pragma once
namespace app
{
	class NetBiosStatHelper{
	public:
		struct NetBiosRecord
		{
			std::wstring name_;
			std::wstring type_;
			std::wstring host_address_;
			uint32 life_;
		};
		struct NetBiosInfoOfOneNode
		{
			std::wstring node_name_;  //eg:�������ӣ�VMware Network Adapter VMnet8��
			std::wstring node_ip_;   // �ڵ�IP��
			std::vector<NetBiosRecord> bios_records_;
		};
		typedef std::vector<NetBiosInfoOfOneNode> NetBiosInformation;

		static bool analyze(CPRecord* rootRecord);
	private:
		static bool  GetNetBiosStatInformation(NetBiosInformation& net_bios_information);
	};
}
