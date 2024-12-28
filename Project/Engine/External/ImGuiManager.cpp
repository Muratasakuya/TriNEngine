#include "ImGuiManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	ImGuiManager classMethods
//============================================================================*/

void ImGuiManager::Init(HWND hwnd, UINT bufferCount, SrvManager* srvManager) {

	assert(srvManager);
	srvManager_ = srvManager;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		GraphicsEngine::GetDevice()->Get(),
		bufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		srvManager_->GetDescriptorHeap(),
		srvManager_->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		srvManager_->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());

	srvManager_->Allocate();

	//===================================================================*/
	//							gui settings
	//===================================================================*/

	// ImGuiのフォント設定
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	const std::string fontFilePath = "Resources/Engine/ImGuiFont/FiraMono-Bold.ttf";
	io.Fonts->AddFontFromFileTTF(fontFilePath.c_str(), 16.0f);

	// 背景色設定
	ImGuiStyle& style = ImGui::GetStyle();

	// テキストの色
	style.Colors[ImGuiCol_Text] = ImVec4(0.64f, 0.64f, 0.64f, 1.0f);

	// ウィンドウの背景色
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.02f, 0.02f, 0.02f, 1.0f);

	// タイトルバーの背景色
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

	// 選ばれているウィンドウのタイトル色
	style.Colors[ImGuiCol_Tab] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
	style.Colors[ImGuiCol_TabSelected] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);
	style.Colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.04f, 0.04f, 0.04f, 1.0f);

	// 選ばれていないウィンドウののタイトル色
	style.Colors[ImGuiCol_TabDimmed] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

	// 間の線
	style.Colors[ImGuiCol_Border] = ImVec4(0.08f, 0.08f, 0.08f, 0.5f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.08f, 0.08f, 0.08f, 0.5f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.08f, 0.08f, 0.08f, 0.5f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.08f, 0.08f, 0.08f, 0.5f);
	style.DockingSeparatorSize = 0.64f;

	// DragFloatなどの色
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);

	// Sliderの持ち手色
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.48f, 0.48f, 0.48f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.0f);

	// CheckBoxの色
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.72f, 0.72f, 0.72f, 1.0f);

	// Headerの色
	style.Colors[ImGuiCol_Header] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	// Buttonの色
	style.Colors[ImGuiCol_Button] = ImVec4(0.16f, 0.16f, 0.16f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.0f);
	// 初手変になるのを防ぐため
	style.Colors[ImGuiCol_ScrollbarGrab].w = 0.0f;

	// 角の丸み
	style.FrameRounding = 1;

}

void ImGuiManager::Begin() {

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End() {

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

void ImGuiManager::Draw(ID3D12GraphicsCommandList* commandList) {

	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager_->GetDescriptorHeap() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

}

void ImGuiManager::Finalize() {

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}