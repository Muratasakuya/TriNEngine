#include "MeshRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>

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

			pipeline = RendererPipelineType::NormalObject3D;
		} else {

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

	if (selectedGameObject_) {
		if (selectedGameObject_->GetName() == gameObject->GetName()) {

			selectedGameObject_ = nullptr;
		}
	}

	gameObjects_.erase(std::remove(gameObjects_.begin(), gameObjects_.end(), gameObject), gameObjects_.end());
}

void MeshRenderer::Clear() {

	gameObjects_.clear();
	selectedGameObject_ = nullptr;
}

void MeshRenderer::SelectGameObject() {

	// 他のObjectが選択されていたら選択解除する
	if (EditorManager::GetSelectedEditor()||
		GameSystem::GameCamera()->SelectedGameCamera()) {

		currentObjectIndex_ = -1;
		selectedGameObject_ = nullptr;
	}

	// 名前ごとにグループ化するためのマップ
	std::unordered_map<std::string, std::vector<std::pair<int, IBaseGameObject*>>> groupedObjects;

	auto GetBaseName = [](const std::string& name) {
		size_t index = name.find_last_not_of("0123456789");
		if (index != std::string::npos && index + 1 < name.size()) {
			return name.substr(0, index + 1); // 数字部分を除外した基本名を取得
		}
		return name;
		};

	for (int i = 0; i < gameObjects_.size(); ++i) {
		if (gameObjects_[i]) {
			std::string baseName = GetBaseName(gameObjects_[i]->GetName());
			groupedObjects[baseName].emplace_back(i, gameObjects_[i]);
		}
	}

	// UI 描画
	if (!groupedObjects.empty()) {
		for (const auto& group : groupedObjects) {

			const std::string& baseName = group.first;
			const auto& objects = group.second;

			if (objects.size() == 1) {
				int index = objects[0].first;
				auto& obj = objects[0].second;

				if (ImGui::Selectable(obj->GetName().c_str(), currentObjectIndex_ == index)) {

					currentObjectIndex_ = index;
					selectedGameObject_ = obj;
				}
			} else {
				// 2つ以上の場合はListで表示
				if (ImGui::TreeNode((baseName + "List").c_str())) {
					for (const auto& item : objects) {

						int index = item.first;
						auto& obj = item.second;

						if (ImGui::Selectable(obj->GetName().c_str(), currentObjectIndex_ == index)) {

							currentObjectIndex_ = index;
							selectedGameObject_ = obj;
						}
					}
					ImGui::TreePop();
				}
			}
		}
	}
}

void MeshRenderer::SelectedImGui() {

	if (!selectedGameObject_) {
		return;
	}

	ImGui::Text(selectedGameObject_->GetName().c_str());
	ImGui::Separator();
	selectedGameObject_->ImGui();

}