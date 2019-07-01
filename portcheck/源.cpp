#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <Tlhelp32.h>
#include <winsock.h>
#include <stdio.h>//sdfsdfsadf
#pragma comment(lib, "ws2_32.lib") //htons()
#include <iostream>
using namespace std;
//http://code1.okbase.net/codefile/NPort.cpp_2014041526113_1.html

#define ANY_SIZE 1



//该 MIB_TCPROW结构包含descibes IPv4的TCP连接信息
typedef struct _MIB_TCPROW_OWNER_PID {

	DWORD dwState;  //TCP状态

	DWORD dwLocalAddr;

	DWORD dwLocalPort;

	DWORD dwRemoteAddr;

	DWORD dwRemotePort;

	DWORD dwOwningPid;

} MIB_TCPROW_OWNER_PID, * PMIB_TCPROW_OWNER_PID;

// 以下为与UDP相关的结构. 
typedef struct tagMIB_UDPEXROW {
	DWORD dwLocalAddr;          // 本地计算机地址.
	DWORD dwLocalPort;          // 本地计算机端口.
	DWORD dwProcessId;
} MIB_UDPEXROW, * PMIB_UDPEXROW;

typedef struct tagMIB_UDPEXTABLE {
	DWORD dwNumEntries;
	MIB_UDPEXROW table[100];    // 任意大小数组变量. 
} MIB_UDPEXTABLE, * PMIB_UDPEXTABLE;


typedef   struct _MIB_TCPTABLE_OWNER_PID {

	DWORD dwNumEntries;

	MIB_TCPROW_OWNER_PID  table[ANY_SIZE];

}MIB_TCPTABLE_OWNER_PID, * PMIB_TCPTABLE_OWNER_PID;

typedef enum _TCP_TABLE_CLASS {
	TCP_TABLE_BASIC_LISTENER,
	TCP_TABLE_BASIC_CONNECTIONS,
	TCP_TABLE_BASIC_ALL,
	TCP_TABLE_OWNER_PID_LISTENER,
	TCP_TABLE_OWNER_PID_CONNECTIONS,
	TCP_TABLE_OWNER_PID_ALL,
	TCP_TABLE_OWNER_MODULE_LISTENER,
	TCP_TABLE_OWNER_MODULE_CONNECTIONS,
	TCP_TABLE_OWNER_MODULE_ALL
} TCP_TABLE_CLASS, * PTCP_TABLE_CLASS;

typedef enum _UDP_TABLE_CLASS {
	UDP_TABLE_BASIC,
	UDP_TABLE_OWNER_PID,
	UDP_TABLE_OWNER_MODULE
} UDP_TABLE_CLASS, * PUDP_TABLE_CLASS;


typedef   DWORD(WINAPI* PGetExtendedTcpTable)(

	PVOID  pTcpTable,

	PDWORD   pdwSize,

	BOOL   bOrder,

	ULONG   ulAf,

	TCP_TABLE_CLASS   TableClass,

	ULONG   Reserved

	);

typedef   DWORD(WINAPI* PGetExtendedUDPTable)(

	PVOID  pUdpTable,

	PDWORD   pdwSize,

	BOOL   bOrder,

	ULONG   ulAf,

	UDP_TABLE_CLASS   TableClass,

	ULONG   Reserved

	);


static char TcpState[][32] = {
	TEXT("???"),
	TEXT("CLOSED"),
	TEXT("LISTENING"),
	TEXT("SYN_SENT"),
	TEXT("SYN_RCVD"),
	TEXT("ESTABLISHED"),
	TEXT("FIN_WAIT1"),
	TEXT("FIN_WAIT2"),
	TEXT("CLOSE_WAIT"),
	TEXT("CLOSING"),
	TEXT("LAST_ACK"),
	TEXT("TIME_WAIT"),
	TEXT("DELETE_TCB")
};



int main() {
	PMIB_TCPTABLE_OWNER_PID pTcpExTable;

	HINSTANCE hLibrary = LoadLibrary("Iphlpapi.dll "); //载入库

	PGetExtendedTcpTable pFuncGetTCPTable;

	pFuncGetTCPTable = (PGetExtendedTcpTable)::GetProcAddress(hLibrary, "GetExtendedTcpTable");

	//获得GetExtendedTcpTable函数
	DWORD dwSize = 0;
	int code = pFuncGetTCPTable(NULL, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);

	pTcpExTable = new MIB_TCPTABLE_OWNER_PID[dwSize];//然后动态分配空间

	pFuncGetTCPTable(pTcpExTable, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0);
	cout << pTcpExTable->dwNumEntries << endl;

	for (int i = 0; i < pTcpExTable->dwNumEntries; i++)
	{
		/*if (pTcpExTable->table[i].dwState == 2) {*/
		cout << htons((WORD)pTcpExTable->table[i].dwLocalPort) << "  ";
		cout << TcpState[pTcpExTable->table[i].dwState] << endl;
		/*}*/

	}

	PMIB_UDPEXTABLE  pUDPExTable;
	PGetExtendedUDPTable pFuncGetUDPTable;

	pFuncGetUDPTable = (PGetExtendedUDPTable)::GetProcAddress(hLibrary, "GetExtendedUdpTable");


	 dwSize = 0;
	 code = pFuncGetUDPTable(NULL, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);

	pUDPExTable = new MIB_UDPEXTABLE[dwSize];//然后动态分配空间

	pFuncGetUDPTable(pUDPExTable, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0);
	cout << pUDPExTable->dwNumEntries << endl;
	cout << "===============================UDP=================================" << endl;
	for (int i = 0; i < pUDPExTable->dwNumEntries; i++)
	{
		/*if (pTcpExTable->table[i].dwState == 2) {*/
		cout << htons((WORD)pUDPExTable->table[i].dwLocalPort) <<endl;
		/*cout << TcpState[pTcpExTable->table[i].dwState] << endl;*/
		/*}*/

	}

}