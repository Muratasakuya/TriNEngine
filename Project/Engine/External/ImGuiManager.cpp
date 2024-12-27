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

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// ImGuiのフォント設定
	ImGuiIO& io = ImGui::GetIO();
	const std::string fontFilePath = "Resources/Engine/ImGuiFont/FiraMono-Bold.ttf";
	io.Fonts->AddFontFromFileTTF(fontFilePath.c_str(), 16.0f);

	// 背景色設定
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.0f);

}

void ImGuiManager::Begin() {

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End() {

	ImGui::Render();

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