#include "ImGuiRenderer.h"

//============================================================================*/
//	include
//============================================================================*/

// EngineView
#include <Game/Utility/GameTimer.h>
// GameView
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Game/System/GameSystem.h>
#include <Game/Editor/Manager/EditorManager.h>

//============================================================================*/
//	ImGuiRenderer classMethods
//============================================================================*/

void ImGuiRenderer::Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle,
	const D3D12_GPU_DESCRIPTOR_HANDLE& demoSceneRenderTextureGPUHandle) {

	// RenderTextureのGpuHandleの取得
	renderTextureGPUHandle_ = renderTextureGPUHandle;
	demoSceneRenderTextureGPUHandle_ = demoSceneRenderTextureGPUHandle;

	windowFlag_ = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

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

	ImGui::Begin("Game", nullptr, windowFlag_);

	const ImVec2 imageSize(768.0f, 432.0f);
	ImGui::Image(ImTextureID(renderTextureGPUHandle_.ptr), imageSize);

	ImGui::End();

	ImGui::Begin("DemoScene", nullptr, windowFlag_);

	ImGui::Image(ImTextureID(demoSceneRenderTextureGPUHandle_.ptr), imageSize);

	ImGui::End();

}

void ImGuiRenderer::EngineView() {

	ImGui::Begin("Process");

	GameSystem::GetCollision()->DisplayCollisionLogs();

	ImGui::End();

	ImGui::Begin("Engine");

	GameTimer::ImGui();

	ImGui::End();

}

void ImGuiRenderer::GameView() {

	ImGui::Begin("GameObject");

	GameSystem::GameCamera()->SelectGameCamera();
	GameSystem::GameLight()->SelectGameLight();
	MeshRenderer::SelectGameObject();

	ImGui::End();

	EditorManager::SelectEditor();

	SpriteRenderer::SelectSprite();

}

void ImGuiRenderer::InspectorView() {

	ImGui::Begin("Inspector");

	MeshRenderer::SelectedImGui();
	SpriteRenderer::SelectedImGui();
	GameSystem::GameCamera()->ImGui();
	GameSystem::GameLight()->ImGui();
	EditorManager::SelectedImGui();

	ImGui::End();
}

void ImGuiRenderer::DisplayMatrix(const std::string& windowName, const Matrix4x4& matrix) {

	auto displayFunction = [windowName, &matrix]() {

		ImGui::Text(windowName.c_str());
		for (int i = 0; i < 4; ++i) {
			ImGui::Text("%.3f %.3f %.3f %.3f",
				matrix.m[i][0], matrix.m[i][1], matrix.m[i][2], matrix.m[i][3]);
		}
		};
	displayFunction();
}

void ImGuiRenderer::RenderTask() {

	Vector3 axis = Vector3(1.0f, 1.0f, 1.0f).Normalize();
	float angle = 0.44f;
	Quaternion quaternion = Quaternion::MakeRotateAxisAngleQuaternion(axis, angle);
	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(quaternion);

	// 値の表示
	ImGui::Begin("MT4");
	DisplayMatrix("rotateMatrix", rotateMatrix);
	ImGui::End();
}