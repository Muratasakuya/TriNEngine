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

	Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(
		Vector3(1.0f, 0.4f, -0.2f).Normalize(), 0.45f);

	Vector3 pointY = Vector3(2.1f, -0.9f, 1.3f);

	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(rotation);
	Vector3 rotateByQuaternion = Quaternion::RotateVector(pointY, rotation);
	Vector3 rotateByMatrix = Vector3::Transform(pointY, rotateMatrix);

	// 値の表示
	ImGui::Begin("MT4");
	DisplayQuaternion(": rotation", rotation);
	DisplayMatrix("rotateMatrix", rotateMatrix);
	DisplayVector3(" : rotateByQuaternion", rotateByQuaternion);
	DisplayVector3(" : rotateByMatrix", rotateByMatrix);

	ImGui::End();
}