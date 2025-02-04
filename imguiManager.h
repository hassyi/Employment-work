#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>

class ImguiManager
{
private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;

	ID3D11RenderTargetView* m_RendererTargetView;
	IDXGISwapChain* m_SwapChain = nullptr;


public:
	ImguiManager() = default;
	~ImguiManager(){}

	void Init();
	void Uninit();

	void ImGuiRendererInit();
	void ImGuiRenderer();
	void DrawImGui();


};