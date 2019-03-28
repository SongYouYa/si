#pragma once

namespace app
{
    class  LogonSessionsInformationHelper
    {
    public:
		struct LogonSessionInformation
		{
			std::wstring logon_session_;
			std::wstring user_name_;
			std::wstring auth_package_;
			std::wstring logon_type_;
			boost::uint32_t session_;
			std::wstring sid_;
			CPTime logon_time_;
			std::wstring logon_server_;
			std::wstring dns_domain_;
			std::wstring upn_;
			std::map<boost::uint32_t, std::wstring> processlist_;
		};
		typedef std::vector<LogonSessionInformation> LogonSessionsInformation;

		static bool analyze(CPRecord* parentRecord);

    private:
		static bool  GetLogonSessionsInformation(LogonSessionsInformation& logon_sessions_information);
		static bool GetAllProcessIds(std::vector<boost::uint32_t>& process_ids);
		static bool GetAllProcesspaths(const std::vector<boost::uint32_t>& process_ids,
                                std::vector<std::wstring>& process_paths, std::vector<PTOKEN_USER>& users);
		static bool GetLogonSessionInfoByLoadLibrary(const std::vector<PTOKEN_USER>& users, const std::vector<boost::uint32_t>& process_ids,
                                              const std::vector<std::wstring>& process_paths, LogonSessionsInformation& logon_sessions_information);
		
    };
}
