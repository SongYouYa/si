#pragma once
#include <IPHlpApi.h>

namespace app
{
    //�ṹ����
    typedef struct tagMIB_TCPEXROW
    {
        DWORD dwState; //tcp����״̬
        DWORD dwLocalAddr; //����ip��ַ
        DWORD dwLocalPort; //�򿪱��ض˿�
        DWORD dwRemoteAddr; //Զ��ip��ַ
        DWORD dwRemotePort; //Զ�̶˿ں�
        DWORD dwProcessId; //���̱�ʶ
    } MIB_TCPEXROW,*PMIB_TCPEXROW;
    typedef struct tagMIB_TCPEXTABLE
    {
        DWORD dwNumEntries;
        MIB_TCPEXROW table[ANY_SIZE];
    } MIB_TCPEXTABLE,*PMIB_TCPEXTABLE;
    typedef struct tagMIB_UDPEXROW
    {
        DWORD dwLocalAddr;//����ip��ַ
        DWORD dwLocalPort;//�򿪱��ض˿�
        DWORD dwProcessId;//���̱�ʶ
    } MIB_UDPEXROW,*PMIB_UDPEXROW;
    typedef struct tagMIB_UDPEXTABLE
    {
        DWORD dwNumEntries;
        MIB_UDPEXROW table[ANY_SIZE];
    } MIB_UDPEXTABLE,*PMIB_UDPEXTABLE;
    //TCP_TABLE_CLASS�ṹ����
    typedef enum
    {
        TCP_TABLE_BASIC_LISTENER,	//�������ڽ������ݵ�TCP�����˿ڵ�MIB_TCPTABLE
        TCP_TABLE_BASIC_CONNECTIONS,//�����ѽ������ӵ�TCP�˿ڵ�MIB_TCPTABLE
        TCP_TABLE_BASIC_ALL,		//����TCP�˿ڵ�MIB_TCPTABLE
        TCP_TABLE_OWNER_PID_LISTENER,//�������ڽ������ݵ�TCP�����˿ڵ�MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_PID_CONNECTIONS,//�����ѽ������ӵ�TCP�˿ڵ�MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_PID_ALL,//����TCP�˿ڵ�MIB_TCPTABLE_OWNER_PID
        TCP_TABLE_OWNER_MODULE_LISTENER,//�������ڽ������ݵ�TCP�����˿ڵ�MIB_TCPTABLE_OWNER_MODULE
        TCP_TABLE_OWNER_MODULE_CONNECTIONS,//�����ѽ������ӵ�TCP�˿ڵ�MIB_TCPTABLE_OWNER_MODULE
        TCP_TABLE_OWNER_MODULE_ALL//����TCP�˿ڵ�MIB_TCPTABLE_OWNER_MODULE
    } TCP_TABLE_CLASS, *PTCP_TABLE_CLASS;
    //UDP_TABLE_CLASS�ṹ����
    typedef enum
    {
        UDP_TABLE_BASIC,
        UDP_TABLE_OWNER_PID,
        UDP_TABLE_OWNER_MODULE
    } UDP_TABLE_CLASS, *PUDP_TABLE_CLASS;
    //GetExtendedTcpTable����ָ�붨��
    typedef DWORD (WINAPI *PGET_EXTENDED_TCP_TABLE)(
        PVOID pTcpTable,
        PDWORD pdwSize,
        BOOL bOrder,
        ULONG ulAf,
        TCP_TABLE_CLASS TableClass,
        ULONG Reserved
    );
    //GetExtendedUdpTable����ָ�붨��
    typedef DWORD (WINAPI *PGET_EXTENDED_UDP_TABLE)(
        PVOID pUdpTable,
        PDWORD pdwSize,
        BOOL bOrder,
        ULONG ulAf,
        UDP_TABLE_CLASS TableClass,
        ULONG Reserved
    );
    //��������
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
