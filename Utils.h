#pragma once

#include <map>
#include <algorithm>
#include <functional>

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"
#include "crappy.h"

struct AimBot_Infomation
{
	ULONG64 Dist;
	Vector3 Postion;
};

struct Game_Info
{
	ULONG64 _Name_Addr;
};

std::string weaponinfo(Player Myself, Game_Info gi);
AimBot_Infomation DrawSkeleton(Player personalle, Player Myself, int Scope, Game_Info gi);
void DrawHealth(Player personalle, Player Myself);
void DrawDist(Player personalle, Player Myself);
void DrawVehicle(Player personalle, Player Myself);
void DrawItem(Player personalle, Player Myself);
ULONG GetSpectator(Player Myself);
Vector3 WorldToScreen(Vector3 WorldLocation);
FCameraCacheEntry GetCameraCache(ULONG64 Camera_Addr);
Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id);
std::string VariableText(const char* format, ...);
//extern ImFont* m_pTabs;
extern ImFont* m_pDefault;
extern ImFont* m_pChinese;
extern float _Test_Type_Float;
extern int s_width;
extern int s_height;
static FCameraCacheEntry Ocameracache;