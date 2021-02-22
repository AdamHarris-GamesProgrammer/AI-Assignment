#pragma once
#include <d3d11.h>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_dx11.h"
#include "Imgui/imgui_impl_win32.h"

class ImGUIManager
{
public:
	ImGUIManager() = default;
	ImGUIManager(ID3D11Device* device, ID3D11DeviceContext* context, HWND& wnd);


	void BeginFrame();

	void EndFrame();

private:
	void SetStyle();
};

