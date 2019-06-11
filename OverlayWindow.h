#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <Dwmapi.h>
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "IMGUI/imgui_impl_win32.h"
#pragma comment( lib,"Dwmapi.lib" )

#include "DX_Show.h"

class OverlayWindow
{
public:
	OverlayWindow()
	{
		_DX_Show = new DX_Show();
	}
	~OverlayWindow() = default;
public:
	bool Init(HINSTANCE hInstance);
public:
	DX_Show *_DX_Show = nullptr;
	HWND _Hwnd;
};

