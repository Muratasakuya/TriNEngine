#include "LightManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/Direction.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	LightManager classMethods
//============================================================================*/

void LightManager::Init() {

	buffer_.Init();

	// AL4
	buffer_.light.directional.intensity = 0.0f;
	buffer_.light.point.intensity = 0.0f;

	// spotを使用
	buffer_.light.spot.pos = Vector3(0.0f, 56.0f, 0.0f);
	buffer_.light.spot.distance = 99.8f;
	buffer_.light.spot.direction = Direction::Down();
	buffer_.light.spot.intensity = 3.2f;
	buffer_.light.spot.decay = 2.18f;

}

void LightManager::Update() {

	buffer_.Update();

}

void LightManager::SelectGameLight() {

	// 他のObjectが選択された場合は選択解除する
	if (GameSystem::GameCamera()->SelectedGameCamera()||
		SpriteRenderer::GetSelectedSprite() ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor()) {
		selectGameLight_ = false;
	}

	bool isSelected = selectGameLight_;

	if (ImGui::Selectable("gameLight", isSelected)) {

		selectGameLight_ = true;
	}
}

void LightManager::ImGui() {

	if (!selectGameLight_ ||
		GameSystem::GameCamera()->SelectedGameCamera() ||
		SpriteRenderer::GetSelectedSprite() ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor()) {
		return;
	}

	if (ImGui::BeginTabBar("Light Tabs")) {
		// Directional Light
		if (ImGui::BeginTabItem("Directional")) {

			ImGui::ColorEdit3("Color", &buffer_.light.directional.color.r);
			ImGui::DragFloat3("Direction", &buffer_.light.directional.direction.x, 0.01f);
			ImGui::DragFloat("Intensity", &buffer_.light.directional.intensity, 0.01f);
			ImGui::EndTabItem();
		}

		// Spot Light
		if (ImGui::BeginTabItem("Spot")) {

			ImGui::ColorEdit3("Color", &buffer_.light.spot.color.r);
			ImGui::DragFloat3("Pos", &buffer_.light.spot.pos.x, 0.01f);
			ImGui::DragFloat3("Direction", &buffer_.light.spot.direction.x, 0.01f);
			ImGui::DragFloat("Distance", &buffer_.light.spot.distance, 0.01f);
			ImGui::DragFloat("Intensity", &buffer_.light.spot.intensity, 0.01f);
			ImGui::DragFloat("Decay", &buffer_.light.spot.decay, 0.01f);
			ImGui::DragFloat("CosAngle", &buffer_.light.spot.cosAngle, 0.01f);
			ImGui::DragFloat("CosFalloffStart", &buffer_.light.spot.cosFalloffStart, 0.01f);
			ImGui::EndTabItem();
		}

		// Point Light
		if (ImGui::BeginTabItem("Point")) {

			ImGui::ColorEdit3("Color", &buffer_.light.point.color.r);
			ImGui::DragFloat3("Pos", &buffer_.light.point.pos.x, 0.01f);
			ImGui::DragFloat("Intensity", &buffer_.light.point.intensity, 0.01f);
			ImGui::DragFloat("Radius", &buffer_.light.point.radius, 0.01f);
			ImGui::DragFloat("Decay", &buffer_.light.point.decay, 0.01f);
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
}