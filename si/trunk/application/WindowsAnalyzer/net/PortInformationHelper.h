#pragma once
#include "PortInformation.h"

namespace app
{
	class PortInformationHelper
    {
    public:
		struct PortRecord
		{
			std::wstring protocol_;              //Э������TCP/UDP
			std::wstring  process_name_;         // ����
			uint32 process_id_;             // ����ID
			std::wstring local_address_;    // ����IP
			std::wstring foreign_address_;  // Զ��IP
			std::wstring  state_;           // ״̬
			std::wstring process_path_;            // ·��
		};
		// TCP ״̬
		typedef std::vector<PortRecord>  PortInformation;

        PortInformationHelper();
        ~PortInformationHelper();
        bool Initial();
        bool LoadAPIOfWindowsXPOrEarlier();
        bool LoadAPIOfWindowsVistaOrLater();
        bool  GetTCPPortInformation(PortInformation& tcp_port_information);
        bool  GetUDPPortInformation(PortInformation& udp_port_information);

		bool analyze(CPRecord* rootRecord);

    private:
        bool GetTCPPortInformationByEarlierAPI(PortInformation& tcp_port_info);
        bool GetTCPPortInformationByLaterAPI(PortInformation& tcp_port_info);
        bool GetUDPPortInformationByLaterAPI(PortInformation& udp_port_info);
        bool GetUDPPortInformationByEarlierAPI(PortInformation& udp_port_info);
        std::wstring GetProcessNameFromPID(DWORD processID);
        std::wstring GetProcessPathFromPID(DWORD processID);
        std::wstring GetPort(unsigned int port);
        std::wstring GetIP(unsigned int ipaddr);
        std::wstring GetTcpState(DWORD state);
        void FreeAPI(void);
    private:
        PALLOCATE_AND_GET_TCPEXTABLE_FROM_STACK allocate_and_gettcpextable_from_stack_ptr_;
        PALLOCATE_AND_GET_UDPEXTABLE_FROM_STACK allocate_and_getudpextable_from_stack_ptr_;
        PMIB_TCPEXTABLE tcpex_table_ptr_;
        PMIB_UDPEXTABLE udpex_table_ptr_;
        PGET_EXTENDED_TCP_TABLE get_extended_tcp_table_ptr_;
        PGET_EXTENDED_UDP_TABLE get_extended_udp_table_ptr_;
        HMODULE hDLL;
    };
}
