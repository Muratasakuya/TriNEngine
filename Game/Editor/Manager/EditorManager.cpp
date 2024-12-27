#include "EditorManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Engine/Renderer/MeshRenderer.h>

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

void EditorManager::SelectEditor(const ImVec2& mainWindowPos) {

	if (ImGuiRenderer::cameraInfoEnable_ ||
		MeshRenderer::GetSelectedObject()) {

		selectedEditor_ = nullptr;
	}

	if (!editors_.empty()) {
		ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 192.0f));
		ImGui::SetNextItemWidth(144.0f);
		if (ImGui::BeginCombo("##EditorsCombo",
			currentEditorIndex_ >= 0 ? editors_[currentEditorIndex_]->GetName().c_str() : "EditorList", ImGuiComboFlags_NoArrowButton)) {
			for (int i = 0; i < editors_.size(); ++i) {
				if (editors_[i]) {
					bool isSelected = (currentEditorIndex_ == i);
					if (ImGui::Selectable(editors_[i]->GetName().c_str(), isSelected)) {

						currentEditorIndex_ = i;
						selectedEditor_ = editors_[i];

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

		ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 198.0f));
		ImGui::Text("No Editor");
	}

}

void EditorManager::SelectedImGui() {

	ImGui::Text(selectedEditor_->GetName().c_str());
	ImGui::Separator();
	selectedEditor_->ImGui();

}