#include "CameraManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	CameraManager classMethhods
//============================================================================*/

void CameraManager::Init() {

	debugCameraEnable_ = false;

	selectGameCamera_ = false;

	// 2D
	camera2D_ = std::make_unique<Camera2D>();
	camera2D_->Init();

	// 3D
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->Init();

	// Follow
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Init(camera3D_->GetProjectionMatrix());

	// Debug
	debugCamera_ = std::make_unique<DebugCamera>();

	// Sun
	sunLightCamera_ = std::make_unique<SunLightCamera>();
	sunLightCamera_->Init();

}

void CameraManager::Update() {

	debugCamera_->Update(camera3D_->GetTranslate(), camera3D_->GetRotate(), camera3D_->GetProjectionMatrix());
	if (debugCamera_->Enable()) {

		camera3D_->SetCamera(debugCamera_->GetViewProjectionMatrix(), debugCamera_->GetCameraMatrix(), debugCamera_->GetTranslate());
	} else {

		followCamera_->Update();
		camera3D_->SetCamera(followCamera_->GetViewProjectionMatrix(), followCamera_->GetCameraMatrix(), followCamera_->GetTranslate());
	}

	sunLightCamera_->Update();

}

void CameraManager::DrawDebug() {

	sunLightCamera_->DrawDebug();

}

void CameraManager::SelectGameCamera() {

	// 他のObjectが選択された場合は選択解除する
	if (ParticleRenderer::GetSelectedParticle() ||
		SpriteRenderer::GetSelectedSprite() ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor()) {
		selectGameCamera_ = false;
	}

	bool isSelected = selectGameCamera_;

	if (ImGui::Selectable("gameCamera", isSelected)) {

		selectGameCamera_ = true;
	}
}

void CameraManager::ImGui() {

	if (!selectGameCamera_ ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor()) {
		return;
	}

	//========================================================================*/
	ImGui::Text("debugCamera");
	ImGui::Checkbox("debugCameraEnable", &debugCameraEnable_);
	if (debugCameraEnable_) {
		debugCamera_->ImGui();
	}
	debugCamera_->SetEnable(debugCameraEnable_);
	//========================================================================*/
	ImGui::Separator();
	ImGui::Text("mainCamera");
	camera3D_->ImGui();
	//========================================================================*/
	ImGui::Separator();
	ImGui::Text("followCamera");
	followCamera_->ImGui();
	//========================================================================*/
	ImGui::Separator();
	ImGui::Text("sunLightCamera");
	sunLightCamera_->ImGui();

}