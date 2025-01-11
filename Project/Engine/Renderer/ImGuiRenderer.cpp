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

void ImGuiRenderer::DisplayVector3(const std::string& windowName, const Vector3& vector) {

	auto displayFunction = [windowName, &vector]() {

		ImGui::Text("%.2f", vector.x);
		ImGui::SameLine();
		ImGui::Text("%.2f", vector.y);
		ImGui::SameLine();
		ImGui::Text("%.2f", vector.z);
		ImGui::SameLine();
		ImGui::Text(windowName.c_str());
		};
	displayFunction();
}

void ImGuiRenderer::RenderTask() {

	Quaternion rotation0 =
		Quaternion::MakeRotateAxisAngleQuaternion(Vector3(0.71f, 0.71f, 0.0f), 0.3f);

	Quaternion rotation1 = Quaternion(-rotation0.x, -rotation0.y, -rotation0.z, -rotation0.w);

	Quaternion interpolate0 = Quaternion::Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Quaternion::Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Quaternion::Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Quaternion::Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Quaternion::Slerp(rotation0, rotation1, 1.0f);

	// 値の表示
	ImGui::Begin("MT4");
	DisplayQuaternion(": interpolate0 Slerp(q0,q1,0.0f)", interpolate0);
	DisplayQuaternion(": interpolate1 Slerp(q0,q1,0.3f)", interpolate1);
	DisplayQuaternion(": interpolate2 Slerp(q0,q1,0.5f)", interpolate2);
	DisplayQuaternion(": interpolate3 Slerp(q0,q1,0.7f)", interpolate3);
	DisplayQuaternion(": interpolate4 Slerp(q0,q1,1.0f)", interpolate4);
	ImGui::End();
}