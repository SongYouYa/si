#include "stdafx.h"
#include "SetPrivilege.h"
//usage in the main()
#if 0
#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

int main(int argc, char *argv[])
{
    HANDLE hProcess;
    HANDLE hToken;
    int dwRetVal=RTN_OK; // assume success from main()

    // show correct usage for kill
    if (argc != 2)
    {
        fprintf(stderr,"Usage: %s [ProcessId]\n", argv[0]);
        return RTN_USAGE;
    }
    HANDLE hToken;
    if(!GetCurrentThreadPrivilegeToken(&hToken))
    {
        return RTN_ERROR;
    }

    // enable SeDebugPrivilege
    if(!SetPrivilege(hToken, SE_DEBUG_NAME, TRUE))
    {
        DisplayError("SetPrivilege");

        // close token handle
        CloseHandle(hToken);

        // indicate failure
        return RTN_ERROR;
    }

   // open the process
    if((hProcess = OpenProcess(
            PROCESS_ALL_ACCESS,
            FALSE,
            atoi(argv[1]) // PID from commandline
            )) == NULL)
    {
        DisplayError("OpenProcess");
        return RTN_ERROR;
    }

    // disable SeDebugPrivilege
    SetPrivilege(hToken, SE_DEBUG_NAME, FALSE);

    if(!TerminateProcess(hProcess, 0xffffffff))
    {
        DisplayError("TerminateProcess");
        dwRetVal=RTN_ERROR;
    }

    // close handles
    CloseHandle(hToken);
    CloseHandle(hProcess);

    return dwRetVal;
}

#endif

bool CPPrivailegeSeting::GetCurrentThreadPrivilegeToken(PHANDLE phToken)
{
    if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, phToken))
    {
        if (GetLastError() == ERROR_NO_TOKEN)
        {
            if (!ImpersonateSelf(SecurityImpersonation))
                return FALSE;

            if(!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, FALSE, phToken)){
                return FALSE;
            }
        }
        else 
        {
            return FALSE;
        }
    }
    return TRUE;
}

bool CPPrivailegeSeting::SetPrivilege(
	HANDLE hToken,  // token handle 
	LPCTSTR Privilege,  // Privilege to enable/disable 
	BOOL bEnablePrivilege  // TRUE to enable. FALSE to disable 
) 
{ 
	TOKEN_PRIVILEGES tp = { 0 }; 
	// Initialize everything to zero 
	LUID luid; 
	DWORD cb=sizeof(TOKEN_PRIVILEGES); 
	if(!LookupPrivilegeValue( NULL, Privilege, &luid ))
		return FALSE; 
	tp.PrivilegeCount = 1; 
	tp.Privileges[0].Luid = luid; 
	if(bEnablePrivilege) { 
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	} else { 
		tp.Privileges[0].Attributes = 0; 
	} 
	AdjustTokenPrivileges( hToken, FALSE, &tp, cb, NULL, NULL ); 
	if (GetLastError() != ERROR_SUCCESS) 
		return FALSE; 

	return TRUE;
}

void DisplayError(
    LPTSTR szAPI    // pointer to failed API name
    )
{
    LPTSTR MessageBuffer;
    DWORD dwBufferLength;

    printf("%s() error!\n", szAPI);

    if(dwBufferLength=FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                GetLastError(),
                GetSystemDefaultLangID(),
                (LPTSTR) &MessageBuffer,
                0,
                NULL
                ))
    {
        DWORD dwBytesWritten;

        // 
        // Output message string on stderr
        // 
        WriteFile(
                GetStdHandle(STD_ERROR_HANDLE),
                MessageBuffer,
                dwBufferLength,
                &dwBytesWritten,
                NULL
                );
        // 
        // free the buffer allocated by the system
        // 
        LocalFree(MessageBuffer);
    }
}