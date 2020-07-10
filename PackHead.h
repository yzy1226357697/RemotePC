#pragma once

enum MyEnum {
	eCmd = '1',
	eKeyboard
};

#pragma pack(push)
#pragma pack(1)

struct  PackHeader{
	unsigned int type;
	unsigned int length;
	char data[0];
};

#pragma pack(pop)