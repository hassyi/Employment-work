#include "imguiManager.h"
#include "main.h"
#include "renderer.h"
#include "imgui_node_Manager.h"
#include "imgui_blueprints.h"


void ImguiManager::Init()
{
	m_Device = Renderer::GetDevice();
	m_DeviceContext = Renderer::GetDeviceContext();
	m_RendererTargetView = Renderer::GetRendererTargetView();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(m_Device, m_DeviceContext);
	ImGui::StyleColorsDark();

	m_NodeManager = new ImGuiNodeManager;
	m_NodeManager->Init();

	//m_BluePrintManager = new ImGuiBluePrint;
	//m_BluePrintManager->Init();
}

void ImguiManager::Uninit()
{
	m_NodeManager->Uninit();
	//m_BluePrintManager->Uninit();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}

void ImguiManager::ImGuiRendererInit()
{
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void ImguiManager::ImGuiRenderer()
{
	
	//m_BluePrintManager->Draw();

	ImGui::Render();
	//m_DeviceContext->OMSetRenderTargets(1, &m_RendererTargetView, nullptr);
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

void ImguiManager::DrawNodeEditor()
{
	//ノードエディタの描画
	m_NodeManager->DrawBehaviorTree();
}

ImGuiWindowFlags ImguiManager::GetWindowFlags()
{
	return
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoBringToFrontOnFocus;
}
