#pragma once
#include <windows.h>
enum MyEnum {
	STOC_SCREENT = 1,
	CTOS_SCREENT

};

#pragma pack(push)
#pragma pack(1)

struct  PackHeader{
	unsigned int type;
	unsigned int length;
	char data[0];
};

#pragma pack(pop)


/*

BOOL sendData(SOCKET s, DWORD code) {
	
	
	PackHeader PH;
	PH.type = code;
	PH.length = 0;
	if (s == INVALID_SOCKET)
		return FALSE;
	int countlen = 0;//发送的总长
	while (countlen < sizeof(code)) {
		int n = send(s, ((char*)&code) + countlen, sizeof(code)-countlen, 0);//单次发送的长度
		if (n <= 0)
			return FALSE;
		else {
			countlen += n;
		}
	}
	return TRUE;
}


BOOL sendData(SOCKET s,DWORD code ,LPCVOID data,DWORD len ) {
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
}*/