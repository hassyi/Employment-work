#include "imguiManager.h"
#include "main.h"
#include "renderer.h"

void ImguiManager::Init()
{
	m_Device = Renderer::GetDevice();
	m_DeviceContext = Renderer::GetDeviceContext();
	m_RendererTargetView = Renderer::GetRendererTargetView();
	m_SwapChain = Renderer::GetSwapChain();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
	ImGui::StyleColorsDark();
}

void ImguiManager::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImguiManager::ImGuiRendererInit()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::ImGuiRenderer()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ImguiManager::DrawImGui()
{
	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Hello World!");

	ImGui::Text("This is some useful text");

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

	if (ImGui::Button("Button")) counter++;

	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGui::Text("f = %f", f);

	ImGui::End();

	ImGui::Render();

}
