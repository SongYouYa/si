#pragma once
#include <IPHlpApi.h>

namespace app
{
    //结构定义
    typedef struct tagMIB_TCPEXROW
    {
        DWORD dwState; //tcp连接状态
        DWORD dwLocalAddr; //本地ip地址
        DWORD dwLocalPort; //打开本地端口
        DWORD dwRemoteAddr; //远程ip地址
        DWORD dwRemotePort; //远程端口号
        DWORD dwProcessId; //进程标识
    } MIB_TCPEXROW,*PMIB_TCPEXROW;
    typedef struct tagMIB_TCPEXTABLE
    {
        DWORD dwNumEntries;
        MIB_TCPEXROW table[ANY_SIZE];
    } MIB_TCPEXTABLE,*PMIB_TCPEXTABLE;
    typedef struct tagMIB_UDPEXROW
    {
        DWORD dwLocalAddr;//本地ip地址
        DWORD dwLocalPort;//打开本地端口
        DWORD dwProcessId;//进程标识
    } MIB_UDPEXROW,*PMIB_UDPEXROW;
    typedef struct tagMIB_UDPEXTABLE
    {
        DWORD dwNumEntries;
        MIB_UDPEXROW table[ANY_SIZE];
    } MIB_UDPEXTABLE,*PMIB_UDPEXTABLE;
    //TCP_TABLE_CLASS结构定义
    typedef enum
    {
        TCP_TABLE_BASIC_LISTENER,	//所有正在接受数据的TCP监听端口的MIB_TCPTABLE
        TCP_TABLE_BASIC_CONNECTIONS,//所有已建立连接的TCP端口的MIB_TCPTABLE
        TCP_TABLE_BASIC_ALL,		//所有TCP端口的MIB_TCPTABLE
        TCP_TABLE_OWNER_PID_LISTENER,//所有正在接受数据的TCP监听端口的MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_PID_CONNECTIONS,//所有已建立连接的TCP端口的MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_PID_ALL,//所有TCP端口的MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_MODULE_LISTENER,//所有正在接受数据的TCP监听端口的MIB_TCPTABLE_OWNER_MODULE
        TCP_TABLE_OWNER_MODULE_CONNECTIONS,//所有已建立连接的TCP端口的MIB_TCPTABLE_OWNER_MODULE
        TCP_TABLE_OWNER_MODULE_ALL//所有TCP端口的MIB_TCPTABLE_OWNER_MODULE
    } TCP_TABLE_CLASS, *PTCP_TABLE_CLASS;
    //UDP_TABLE_CLASS结构定义
    typedef enum
    {
        UDP_TABLE_BASIC,
        UDP_TABLE_OWNER_PID,
        UDP_TABLE_OWNER_MODULE
    } UDP_TABLE_CLASS, *PUDP_TABLE_CLASS;
    //GetExtendedTcpTable函数指针定义
    typedef DWORD (WINAPI *PGET_EXTENDED_TCP_TABLE)(
        PVOID pTcpTable,
        PDWORD pdwSize,
        BOOL bOrder,
        ULONG ulAf,
        TCP_TABLE_CLASS TableClass,
        ULONG Reserved
    );
    //GetExtendedUdpTable函数指针定义
    typedef DWORD (WINAPI *PGET_EXTENDED_UDP_TABLE)(
        PVOID pUdpTable,
        PDWORD pdwSize,
        BOOL bOrder,
        ULONG ulAf,
        UDP_TABLE_CLASS TableClass,
        ULONG Reserved
    );
    //函数声明
    typedef DWORD (WINAPI *PALLOCATE_AND_GET_TCPEXTABLE_FROM_STACK)(
        PMIB_TCPEXTABLE *pTcpTable,
        BOOL bOrder,
        HANDLE heap,
        DWORD zero,
        DWORD flags
    );
    typedef DWORD (WINAPI *PALLOCATE_AND_GET_UDPEXTABLE_FROM_STACK)(
        PMIB_UDPEXTABLE *pUdpTable,
        BOOL bOrder,
        HANDLE heap,
        DWORD zero,
        DWORD flags
    );
}
