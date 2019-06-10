#include "Utils.h"
#include "Memory.h"
#include <d3d11.h>
#include <sstream>



//defining data
int s_width = 2560;
int s_height = 1440;
LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
HWND hWnd;
HWND twnd;
MARGINS  margin = { 0,0,s_width,s_height };
LPD3DXFONT pFont;
//ImFont* m_pTabs = 0;
ID3DXLine* d3dLine;

list<int> upper_part = { Bones::neck_01, Bones::Head, Bones::forehead };


list<int> right_arm = { Bones::neck_01, Bones::upperarm_r, Bones::lowerarm_r, Bones::hand_r };
list<int> left_arm = { Bones::neck_01, Bones::upperarm_l, Bones::lowerarm_l, Bones::hand_l };
list<int> spine = { Bones::neck_01, Bones::spine_01, Bones::spine_02, Bones::pelvis };
list<int> lower_right = { Bones::pelvis, Bones::thigh_r, Bones::calf_r, Bones::foot_r };
list<int> lower_left = { Bones::pelvis, Bones::thigh_l, Bones::calf_l, Bones::foot_l };
list<list<int>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };


list<int> right_arm_female = { Bones::neck_01, Bones::index_04_r_MBONLY, Bones::middle_01_r, Bones::middle_02_r };
list<int> left_arm_female = { Bones::neck_01, Bones::index_04_l_MBONLY, Bones::middle_01_l, Bones::middle_02_l };
list<int> lower_right_female = { Bones::pelvis, Bones::skirt_f_01, Bones::skirt_f_02, Bones::skirt_f_03 };
list<int> lower_left_female = { Bones::pelvis, Bones::calf_twist_01_r, Bones::thigh_twist_01_r, Bones::Slot_SideArm };
list<list<int>> skeleton_female = { upper_part, right_arm_female, left_arm_female, spine, lower_right_female, lower_left_female };


ImFont* m_pDefault = NULL;
ImFont* m_pChinese = NULL;
float _Test_Type_Float = 0.f;

static FCameraCacheEntry Ocameracache;

ULONG _Test_ULONG = 0;
float _Test_FLOAT;
ULONG64 _Test_ULONG64 = 0;
Vector3 _Test_Vector3;

FCameraCacheEntry Get_Ocameracache()
{
	return Ocameracache;
}


D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

Vector3 WorldToScreen(Vector3 WorldLocation)
{
	Vector3 Screenlocation = Vector3(0, 0, 0);

	auto POV = Ocameracache.POV;
	Vector3 Rotation = POV.Rotation; // FRotator

	D3DMATRIX tempMatrix = Matrix(Rotation); // Matrix

	Vector3 vAxisX, vAxisY, vAxisZ;

	vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - POV.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	float FovAngle = POV.FOV;
	float ScreenCenterX = s_width / 2.0f;
	float ScreenCenterY = s_height / 2.0f;

	Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
	Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

	return Screenlocation;
}




void GetCameraCache(ULONG64 Camera_Addr)
{
	Vector3 temp_rot;
	Memory::_This->Read_Memory(Camera_Addr + TSL_CAMERAROT, &temp_rot, sizeof(Vector3));
	Vector3 temp_pos;
	Memory::_This->Read_Memory(Camera_Addr + TSL_CAMERAPOS, &temp_pos, sizeof(Vector3));
	float temp_fov;
	Memory::_This->Read_Memory(Camera_Addr + TSL_CAMERAFOV, &temp_fov, sizeof(float));
	Ocameracache.POV.Rotation = temp_rot;
	Ocameracache.POV.Location = temp_pos;
	Ocameracache.POV.FOV = temp_fov;
}



D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}


FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	FTransform temp;
	Memory::_This->Read_Memory(Memory::_This->Read_Memory(mesh + TSL_BONE, _Test_ULONG64) + index * 0x30,&temp, sizeof(FTransform));
	return temp;
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld;
	Memory::_This->Read_Memory(mesh + TSL_BONEARRAY, &ComponentToWorld, sizeof(FTransform));
	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}


ULONG GetSpectator(Player Myself)
{
	return Memory::_This->Read_Memory(Myself.Addr + TSL_AUDIENCE, _Test_ULONG);
}

void FillRGB(float x, float y, float w, float h, int r, int g, int b, int a)
{
	D3DXVECTOR2 vLine[2];

	d3dLine->SetWidth(w);

	vLine[0].x = x + w / 2;
	vLine[0].y = y;
	vLine[1].x = x + w / 2;
	vLine[1].y = y + h;

	d3dLine->Begin();
	d3dLine->Draw(vLine, 2, D3DCOLOR_RGBA(r, g, b, a));
	d3dLine->End();
}

void DrawBar(int x, int y, float value, float dist,float lenght)
{
	float l, r, g;
	l = value / 100;
	r = lenght / 600;
	x = x + lenght / 6;
	

	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x , y + l*lenght), ImVec2(x + r*20, y + lenght), D3DCOLOR_ARGB(255, 0, 0, 255));
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + r*20, y + l*lenght), D3DCOLOR_ARGB(255, 0, 255, 255));
}

void DrawItem(Player personalle, Player Myself, int _Item_Scope)
{
	personalle.position = Memory::_This->Read_Memory(personalle.Addr + TSL_Position, _Test_Vector3);

	//find distance
	Vector3 FootPos = personalle.position;
	float dist = FootPos.Distance(Ocameracache.POV.Location) / 100;
	if (dist > _Item_Scope) // 超速
		return;

	Vector3 w2s = WorldToScreen(personalle.position);
}

Vector3 _Last = Vector3(0, 0, 0);
