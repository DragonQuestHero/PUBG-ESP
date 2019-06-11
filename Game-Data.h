#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

#include "Addrs-Offset.h"
#include "Memory.h"
#include "Utils.h"

//all IDA reversal functions, just in case the assembly code isn't reliable enough.

#define BYTEn(x, n) (*((BYTE *)&(x) + n))
#define WORDn(x, n) (*((WORD *)&(x) + n))
#define DWORDn(x, n) (*((DWORD *)&(x) + n))

#define IDA_LOBYTE(x) BYTEn(x, 0)
#define IDA_LOWORD(x) WORDn(x, 0)
#define IDA_LODWORD(x) DWORDn(x, 0)
#define IDA_HIBYTE(x) BYTEn(x, 1)
#define IDA_HIWORD(x) WORDn(x, 1)
#define IDA_HIDWORD(x) DWORDn(x, 1)

#define BYTE1(x) BYTEn(x, 1)
#define BYTE2(x) BYTEn(x, 2)
#define WORD1(x) WORDn(x, 1)
#define DWORD1(x) DWORDn(x, 1)
#define WORD2(x)   WORDn(x,  2)

#define OFFSET_IDCALC              0x3E20
static uint8_t __ROL1__(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << count) | (x >> (8 - count));
}
static uint16_t __ROL2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << count) | (x >> (16 - count));
}
static uint32_t __ROL4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << count) | (x >> (32 - count));
}
static uint64_t __ROL8__(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << count) | (x >> (64 - count));
}
static uint8_t __ROR1__(uint8_t x, unsigned int count) {
	count %= 8;
	return (x << (8 - count)) | (x >> count);
}
static uint16_t __ROR2__(uint16_t x, unsigned int count) {
	count %= 16;
	return (x << (16 - count)) | (x >> count);
}
static uint32_t __ROR4__(uint32_t x, unsigned int count) {
	count %= 32;
	return (x << (32 - count)) | (x >> count);
}
static uint64_t __ROR8__(uint64_t x, unsigned int count) {
	count %= 64;
	return (x << (64 - count)) | (x >> count);
}

class Game_Data
{
public:
	Game_Data()
	{
		_Memory = new Memory();
	}
	~Game_Data() = default;
public:
	bool Hook_Start();
	void Open_NoRecoil();
	void Close_NoRecoil();
	std::vector<Player> Get_Data(bool bVehicle, bool bWeapon, bool bArmor, bool bHealth, bool bThrowable, bool bParts, bool bTrace);
	std::vector<Player> Display_Vehicle();
public:
	Memory * _Memory = nullptr;
	bool _overrides = true;
	ULONG64 _Myself_Addr = 0;
private:
	ULONG64 _PID = 0;
	inline uint64_t decryptFunction(uint64_t ptr) // UPDATE this for predictions to work.
	{
		uint64_t v15;
		IDA_LODWORD(v15) = __ROR4__((__ROR4__(ptr ^ 0x793303BD, 16) + 345261228) ^ 0x143303BD, 16) ^ 0x149444AC;
		IDA_HIDWORD(v15) = __ROR4__((__ROR4__(IDA_HIDWORD(ptr) ^ 0xDE62C6E8, 8) + 321590060) ^ 0x7962C6E8, 8) ^ 0xECD4ECD4;

		/*IDA_LODWORD(v15) = __ROL4__(__ROL4__(ptr - 0x5188FC43, 16) + 0x5360C964, 16) ^ 0x9E9C5A7;
		IDA_HIDWORD(v15) = __ROR4__(__ROR4__((ptr >> 32) + 0x13A6C6E8, 8) + 0x36ED4EEF, 8) ^ 0x77B977F9;*/
		return v15;
	}
private:
	ULONG64 _Object_Table_Addr = 0;
	ULONG64 _World_Addr = 0;
	ULONG64 _Name_Addr = 0;
	char _NoRecoil1[8];
	char _NoRecoil2[8];
	char _NoRecoil_Left[4];
	char _NoRecoil_UP[4];
	char _ReHold[2];
public:
	ULONG64 _Object_Table_Data = 0;
	ULONG64 _World_Data = 0;
	ULONG64 _Myself_Data = 0;
	ULONG64 _Name_Data = 0;
	ULONG64 _TslMode_Addr = 0;
	std::vector<Player> _Player_Vector;
	std::vector<Player> _Vehicle_Vector;
	std::vector<Player> _Item_Vector;
	//std::vector<std::string> _Vehicle_Name_Vector;
	Player _Player_Myself_Data;
	ULONG64 钩子地址;
	DOUBLE JE地址;
	ULONG64 空白地址;
	ULONG 钩子数据;
	ULONG64 Code数据;

	ULONG64 JNE_Addr;
	ULONG64 SUB_Addr;
	bool bOnce = true;
public:
	ULONG _Test_ULONG = 0;
	float _Test_Float = 0;
	ULONG64 _Test_ULONG64 = 0;
	Vector3 _Test_Vector3 = Vector3();
	char* _Test_char = 0;
};

