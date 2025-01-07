#include "EditorManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	EditorManager classMethods
//============================================================================*/

std::vector<BaseEditor*> EditorManager::editors_ = {};
BaseEditor* EditorManager::selectedEditor_ = nullptr;
int EditorManager::currentEditorIndex_ = 0;

void EditorManager::SetEditor(BaseEditor* editor) {

	if (!editor) {
		return;
	}

	editors_.emplace_back(editor);
}

void EditorManager::EraseEditor(BaseEditor* editor) {

	editors_.erase(std::remove(editors_.begin(), editors_.end(), editor), editors_.end());
}

void EditorManager::Clear() {

	editors_.clear();
	selectedEditor_ = nullptr;
}

void EditorManager::SelectEditor() {

	ImGui::Begin("Editor");

	// 他のオブジェクトが選択されていたら選択解除
	if (SpriteRenderer::GetSelectedSprite() ||
		MeshRenderer::GetSelectedObject() ||
		ParticleRenderer::GetSelectedParticle() ||
		GameSystem::GameLight()->SelectedGameLight() ||
		GameSystem::GameCamera()->SelectedGameCamera()) {
		selectedEditor_ = nullptr;
		currentEditorIndex_ = -1;
	}

	// UI 描画
	if (!editors_.empty()) {
		for (int i = 0; i < editors_.size(); ++i) {
			auto& editor = editors_[i];
			if (!editor) {
				continue;
			}

			// エディタをリスト表示
			bool isSelected = (currentEditorIndex_ == i);
			if (ImGui::Selectable(editor->GetName().c_str(), isSelected)) {

				currentEditorIndex_ = i;
				selectedEditor_ = editor;
			}
		}
	}

	ImGui::End();

}

void EditorManager::SelectedImGui() {

	if (!selectedEditor_) {
		return;
	}

	ImGui::Text(selectedEditor_->GetName().c_str());
	ImGui::Separator();
	selectedEditor_->ImGui();

}