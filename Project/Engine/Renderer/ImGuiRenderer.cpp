#include "ImGuiRenderer.h"

//============================================================================*/
//	include
//============================================================================*/

// EngineView
#include <Game/Utility/GameTimer.h>
// GameView
#include <Engine/Renderer/MeshRenderer.h>
#include <Game/System/GameSystem.h>
#include <Game/Editor/Manager/EditorManager.h>

//============================================================================*/
//	ImGuiRenderer classMethods
//============================================================================*/

D3D12_GPU_DESCRIPTOR_HANDLE ImGuiRenderer::renderTextureGPUHandle_ = {};
ImGuiWindowFlags ImGuiRenderer::windowFlag_ = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

bool ImGuiRenderer::testBool_ = false;
int ImGuiRenderer::testInt_ = 0;
float ImGuiRenderer::testFloat_ = 0.0f;

void ImGuiRenderer::Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle) {

	// RenderTextureのGpuHandleの取得
	renderTextureGPUHandle_ = renderTextureGPUHandle;

}

void ImGuiRenderer::Render() {
#ifdef _DEBUG
	//==================================================================================================================*/
	//* DockTarget *//
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID, ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);
	//==================================================================================================================*/
	//* Window *//
	MainWindow();
	//==================================================================================================================*/
	//* Engine *//
	EngineView();
	//==================================================================================================================*/
	//* Game *//
	GameView();
	//==================================================================================================================*/
	//* Inspector *//
	InspectorView();
#endif // _DEBUG
}

void ImGuiRenderer::MainWindow() {

	ImGui::Begin("GameView", nullptr, windowFlag_);

	const ImVec2 imageSize(768.0f, 432.0f);
	ImGui::Image(ImTextureID(renderTextureGPUHandle_.ptr), imageSize);

	ImGui::End();

}

void ImGuiRenderer::EngineView() {

	ImGui::Begin("Engine", nullptr, windowFlag_);

	GameTimer::ImGui();

	ImGui::End();
}

void ImGuiRenderer::GameView() {

	ImGui::Begin("GameObject");

	GameSystem::GameCamera()->SelectGameCamera();
	MeshRenderer::SelectGameObject();

	ImGui::End();

	EditorManager::SelectEditor();

}

void ImGuiRenderer::InspectorView() {

	ImGui::Begin("Inspector");

	MeshRenderer::SelectedImGui();
	GameSystem::GameCamera()->ImGui();
	EditorManager::SelectedImGui();

	ImGui::End();
}