#include "MeshRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>

//============================================================================*/
//	MeshRenderer classMethods
//============================================================================*/

std::vector<IBaseGameObject*> MeshRenderer::gameObjects_ = {};
IBaseGameObject* MeshRenderer::selectedGameObject_ = nullptr;
int MeshRenderer::currentObjectIndex_ = 0;

void MeshRenderer::RenderShadowDepth() {

	for (const auto& gameObject : gameObjects_) {

		// 深度に書き込むObjectのみ描画
		if (gameObject->GetDrawShadowEnable() &&
			gameObject->GetDrawEnable()) {

			gameObject->DrawShadowDepth();
		}
	}

}

void MeshRenderer::Render() {

	RendererPipelineType pipeline;

	for (const auto& gameObject : gameObjects_) {

		if (gameObject->GetDrawShadowEnable()) {

			// このObjectには影を落とさない
			pipeline = RendererPipelineType::NormalObject3D;
		} else {

			// このObjectに影を落とす
			pipeline = RendererPipelineType::TargetShadowObject3D;
		}

		if (gameObject->GetDrawEnable()) {

			gameObject->Draw(pipeline);
		}
	}

}

void MeshRenderer::SetGameObject(IBaseGameObject* gameObject) {

	if (!gameObject) {
		return;
	}

	gameObjects_.emplace_back(gameObject);
}

void MeshRenderer::EraseGameObject(IBaseGameObject* gameObject) {

	gameObjects_.erase(std::remove(gameObjects_.begin(), gameObjects_.end(), gameObject), gameObjects_.end());
}

void MeshRenderer::Clear() {

	gameObjects_.clear();
	selectedGameObject_ = nullptr;
}

void MeshRenderer::SelectGameObject(const ImVec2& mainWindowPos) {

	if (ImGuiRenderer::cameraInfoEnable_ ||
		EditorManager::GetSelectedEditor()) {

		selectedGameObject_ = nullptr;
	}

	if (!gameObjects_.empty()) {
		ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 2.0f));
		ImGui::SetNextItemWidth(144.0f);
		if (ImGui::BeginCombo("##GameObjectsCombo",
			currentObjectIndex_ >= 0 ? gameObjects_[currentObjectIndex_]->GetName().c_str() : "GameObjectList", ImGuiComboFlags_NoArrowButton)) {
			for (int i = 0; i < gameObjects_.size(); ++i) {
				if (gameObjects_[i]) {
					bool isSelected = (currentObjectIndex_ == i);
					if (ImGui::Selectable(gameObjects_[i]->GetName().c_str(), isSelected)) {

						currentObjectIndex_ = i;
						selectedGameObject_ = gameObjects_[i];

						ImGuiRenderer::cameraInfoEnable_ = false;

					}
					if (isSelected) {

						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	} else {

		ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 2.0f));
		ImGui::Text("No GameObject");
	}

}

void MeshRenderer::SelectedImGui() {

	ImGui::Text(selectedGameObject_->GetName().c_str());
	ImGui::Separator();
	selectedGameObject_->ImGui();

}