#pragma once


namespace net
{
	struct NetcardInfo
	{
		std::string cardName;
		std::string ipv4;
		std::string netmask;
	};
	typedef std::vector<NetcardInfo> NetcardInfos;

	class CPNetwork
	{
	public:
		//获取所有网卡信息
		static void getNetCards(std::vector<std::string>& cardNames);
		static void getNetInfos(NetcardInfos& netInfos);
		static bool setIp(const std::string& cardName, const std::string& ip,
			const std::string& mask = "255.255.255.0");
	};
}
