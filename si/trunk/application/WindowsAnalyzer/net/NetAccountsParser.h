#pragma

namespace app
{
	struct NetAccountsInfo
	{
		std::wstring itemName_;
		std::wstring itemValue_;
	};
	typedef std::vector<NetAccountsInfo> NetAccountsInfos;

	class CNetAccountsParser
	{
	public:
		CNetAccountsParser();
		~CNetAccountsParser();
		static bool analyze(CPRecord* parentRecord);

	private:
		static bool Parse(NetAccountsInfos& infos);
	};
}

