#pragma once

#ifdef __GNUC__
#include <netinet/in.h>
#endif

namespace auth
{
	class CPAuthClientHandler;
	class CPAuthClient
	{
	public:

		enum AuthClientResult
		{
			ACR_AUTH_APPLY_APPROVED,
			ACR_AUTH_APPLY_TIMEOUT,
			ACR_AUTH_NO_MORE_LICENSE,
			ARC_AUTH_INTERNAL_ERROR,
			ARC_AUTH_SERVER_DISCONNECTED,
			ARC_AUTH_USE_TIME_EXPIRED,
			ARC_AUTH_SERVICE_TIME_EXPIRED,
			ARC_AUTH_UNKNOWN
		};

		CPAuthClient(int port);

		bool init();

		virtual ~CPAuthClient();

		AuthClientResult ApplyAuth();
		bool			 KeepAlive();

		void SetHandler(CPAuthClientHandler* handler);
		CPAuthClientHandler* GetHandler();

		std::string GetResultString(AuthClientResult result);

		const static int MAX_KEEPALIVE_FAILED_COUNT_ALLOWED = 5;

	private:
		static DWORD WINAPI ApplyAuthThreadProc(LPVOID lpParameter);
#ifdef __linux__
        static void* LinuxApplyAuthWrapper(LPVOID lpParameter);
#endif

		//多网卡的情况下，要向每个网卡广播
		//特别是在安装了虚拟机的情况下，会多出一些虚拟网卡
		std::vector<SOCKET> m_sockets;
		SOCKET		m_socket;//可以申请到授权的socket
		sockaddr_in	m_addrto;
		CPAuthClientHandler* m_handler;
		int			m_nPort;
		bool	    m_bApproved;
	private:
		int m_keepAliveFailedCount;

	};
}
