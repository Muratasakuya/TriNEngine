#include "ImGuiRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Utility/Direction.h>

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

void ImGuiRenderer::DisplayQuaternion(const std::string& windowName, const Quaternion& quaternion) {

	auto displayFunction = [windowName, &quaternion]() {

		ImGui::Text("%.2f", quaternion.x);
		ImGui::SameLine();
		ImGui::Text("%.2f", quaternion.y);
		ImGui::SameLine();
		ImGui::Text("%.2f", quaternion.z);
		ImGui::SameLine();
		ImGui::Text("%.2f", quaternion.w);
		ImGui::SameLine();
		ImGui::Text(windowName.c_str());
		};
	displayFunction();
}

void ImGuiRenderer::RenderTask() {

	Quaternion q1 = Quaternion(2.0f, 3.0f, 4.0f, 1.0f);
	Quaternion q2 = Quaternion(1.0f, 3.0f, 5.0f, 2.0f);

	Quaternion identity = Quaternion::IdentityQuaternion();
	Quaternion conj = Quaternion::Conjugate(q1);
	Quaternion inv = Quaternion::Inverse(q1);
	Quaternion normal = Quaternion::Normalize(q1);
	Quaternion mul1 = Quaternion::Multiply(q1, q2);
	Quaternion mul2 = Quaternion::Multiply(q2, q1);
	float norm = Quaternion::Norm(q1);

	// 値の表示
	ImGui::Begin("MT4");
	DisplayQuaternion(" : Identity", identity);
	DisplayQuaternion(" : Conjugate", conj);
	DisplayQuaternion(" : Inverse", inv);
	DisplayQuaternion(" : Normalize", normal);
	DisplayQuaternion(" : Multiply(q1, q2)", mul1);
	DisplayQuaternion(" : Multiply(q2, q1)", mul2);
	ImGui::Text("%.2f : Norm", norm);
	ImGui::End();
}