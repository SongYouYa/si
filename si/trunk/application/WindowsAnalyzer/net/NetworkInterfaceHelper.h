#pragma once
namespace app
{
	class  NetworkInterfaceHelper
	{
	public:
		struct NetAdapterInformation{
			std::wstring friendname_; //OK: GetAdaptersInfo(),GetFriendName().
			std::wstring connection_dns_suffix_;
			std::wstring description_; //OK: GetAdaptersInfo()
			std::wstring physical_address_; //OK: GetAdaptersInfo()
			std::wstring dhcp_enable_;	//OK: GetAdaptersInfo()
			std::wstring ip_autoconfiguration_;
			std::wstring ip_address_;//OK: GetAdaptersInfo()
			std::wstring subnetmask_;//OK: GetAdaptersInfo()
			std::wstring default_gateway_;//OK: GetAdaptersInfo()
			std::wstring dhcp_server_;
			std::wstring dns_server_;	//OK: register
			CPTime lease_obtained_;
			CPTime lease_expires_;
		};
		typedef std::vector<NetAdapterInformation>  NetAdaptersInformation;

		struct NetInterfaceInformation
		{
			std::wstring net_config_name_;	//OK:IsWin2000().
			std::wstring host_name_;		//OK: GetNetworkParams()
			std::wstring primary_dns_suffix_;
			std::wstring node_type_;		//OK: GetNetworkParams()
			std::wstring ip_route_enable_;	//OK: GetNetworkParams()
			std::wstring wins_proxy_enable_;	//OK: GetNetworkParams()
			NetAdaptersInformation adapters_info_;
		};
		static bool analyze(CPRecord* rootRecord);
	private:
		//����������������
		static bool ParseNetworkInfos(NetInterfaceInformation& net_interface_information);
		//��������������
		static bool ParseAdapters(NetAdaptersInformation& infos);
		//��������������
		static bool myGetPerAdapterInfo(ULONG index, std::wstring& dnsServer, std::wstring& wstrIsAutoConfig);
		//����IP����������
		static bool myGetIpAddrAndSubMask(DWORD ifIndex, std::wstring& wstrIpAddr, std::wstring& wstrSubNet);
		//����������������һЩ����
		static bool myGetAdaptersInfo(DWORD ifIndex, std::wstring& wstrGetway, std::wstring& wstrDhcpEnable,
			std::wstring& wstrDhcpServer, CPTime& leaseObtainedTime, CPTime& leaseExpireTime);
		//��ȡ�����������ַ���
		static std::wstring GetAdapterTypeStr(ULONG ifType);
	};
}