#include "Game-Data.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


bool Game_Data::Hook_Start()
{
	_Memory->Init();
	_TslMode_Addr = _Memory->Get_Model_Addr();

	ULONG64 temp_object_table_addr = _TslMode_Addr + TSL_OBJECT_TALBE;
	ULONG64 temp_world_addr = _TslMode_Addr + TSL_WORLD;
	ULONG64 temp_myself_addr = _TslMode_Addr + TSL_MYSELF;
	ULONG64 temp_name_addr = _TslMode_Addr + TSL_NAME;
	return true;
}

void Game_Data::Open_NoRecoil()
{
	_Memory->Write_Memory(_TslMode_Addr + TSL_NORECOIL_UP, TEMP_NORECOIL_UP, 4);
	
}

void Game_Data::Close_NoRecoil()
{
	_Memory->Write_Memory(_TslMode_Addr + TSL_NORECOIL_UP, _NoRecoil_UP, 4);
}

void Game_Data::FlashItem(ULONG64 temp_point, std::string world_name_char)
{
	//儲存物品，優化賊低
}

void Game_Data::FlashVehicle(ULONG64 temp_point, std::string world_name_char)
{
	//儲存車輛
	// 把Vehicle放在最前面能讓他短路。
}

void Game_Data::Get_Data(bool Vehicle, bool Item)
{
	_Player_Vector.clear();
	_Vehicle_Vector.clear();
	_Item_Vector.clear();
	_Object_Table_Data = _Memory->Read_Memory(_Object_Table_Addr, _Object_Table_Data);
	_World_Data = _Memory->Read_Memory(_World_Addr, _World_Data);
	_Myself_Data = _Memory->Read_Memory(_Myself_Addr, _Myself_Data);
	_Name_Data = _Memory->Read_Memory(_Name_Addr, _Name_Data);

	//find localplayer content
	_Player_Myself_Data.Addr = _Myself_Data;

}

