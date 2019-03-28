#include "stdafx.h"
#include "sinetwork/PShareFolder.h"

#ifdef _MSC_VER
#pragma comment(lib, "Mpr.lib")
#endif

namespace net
{
#ifdef _MSC_VER
	bool CPShareFolder::openShareFolder(const std::wstring& remoteDir,
		const std::wstring& userName, const std::wstring& password)
	{
		NETRESOURCE nr;
		nr.dwType = RESOURCETYPE_ANY;
		nr.lpLocalName = NULL; //L"R:"
		nr.lpRemoteName = (LPWSTR)remoteDir.c_str();
		nr.lpProvider = NULL;

		DWORD dwFlags = CONNECT_TEMPORARY;
		
		DWORD ret = WNetAddConnection2(&nr, password.c_str()/*NULL*/, userName.c_str()/*NULL*/, dwFlags);
		if (ret == NO_ERROR) {
			return true;
		}
		else if (ret == ERROR_SESSION_CREDENTIAL_CONFLICT) {
			//如果通过其他方式以及连接过，输入用户名和密码重新连接时,会有此错误；去掉用户名密码后，反而会成功
			ret = WNetAddConnection2(&nr, NULL, NULL, dwFlags);
			if (ret == NO_ERROR) {
				return true;
			}
		}
		return false;
	}
#endif

#ifdef __GNUC__
	bool CPShareFolder::mountShareFolder(const std::wstring& remoteDir,
		const std::wstring& userName, const std::wstring& password,
		const std::wstring& localDir)
	{
#ifdef __linux__
		std::wstring cmd = L"mount.cifs \"" + remoteDir + L"\" \"" + localDir + L"\" -o user="
			+ userName + L",password=" + password;
        int exitCode = 0;
		bool ret = CPProcess::createProcess(cmd, L"", NULL, &exitCode);
		if (!ret || exitCode != 0) {
			return false;
		}
		return true;
#else
        return false;
#endif
	}
#endif
}
