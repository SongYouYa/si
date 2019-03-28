#pragma once

class CPPrivailegeSeting
{
public:
	CPPrivailegeSeting();
	~CPPrivailegeSeting();
	static bool GetCurrentThreadPrivilegeToken(PHANDLE phToken);
	static bool SetPrivilege(
		HANDLE hToken,          // token handle
		LPCTSTR Privilege,      // Privilege to enable/disable
		BOOL bEnablePrivilege   // TRUE to enable.  FALSE to disable
		);
	static void DisplayError(LPTSTR szAPI);
private:

};