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
		//解析网络总体设置
		static bool ParseNetworkInfos(NetInterfaceInformation& net_interface_information);
		//解析所有适配器
		static bool ParseAdapters(NetAdaptersInformation& infos);
		//解析单个适配器
		static bool myGetPerAdapterInfo(ULONG index, std::wstring& dnsServer, std::wstring& wstrIsAutoConfig);
		//解析IP和子网掩码
		static bool myGetIpAddrAndSubMask(DWORD ifIndex, std::wstring& wstrIpAddr, std::wstring& wstrSubNet);
		//解析单个适配器的一些参数
		static bool myGetAdaptersInfo(DWORD ifIndex, std::wstring& wstrGetway, std::wstring& wstrDhcpEnable,
			std::wstring& wstrDhcpServer, CPTime& leaseObtainedTime, CPTime& leaseExpireTime);
		//获取适配器类型字符串
		static std::wstring GetAdapterTypeStr(ULONG ifType);
	};
}