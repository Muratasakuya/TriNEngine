#include "SpriteRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/ParticleRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	SpriteRenderer classMethods
//============================================================================*/

std::vector<BaseSprite*> SpriteRenderer::sprites_ = {};
BaseSprite* SpriteRenderer::selectedSprite_ = nullptr;
int SpriteRenderer::currentSpriteIndex_ = 0;

void SpriteRenderer::Render() {

	// Typeごとに分別
	auto partitionIt = std::partition(sprites_.begin(), sprites_.end(), [](BaseSprite* sprite) {
		return sprite->GetDrawType() != SpriteDrawType::Transition; });

	// None
	for (auto it = sprites_.begin(); it != partitionIt; ++it) {
		(*it)->Draw();
	}

	// Transition
	for (auto it = partitionIt; it != sprites_.end(); ++it) {
		(*it)->Draw();
	}

}

void SpriteRenderer::SetSprite(BaseSprite* sprite) {

	if (!sprite) {
		return;
	}

	sprites_.emplace_back(sprite);
}

void SpriteRenderer::EraseSprite(BaseSprite* sprite) {

	if (selectedSprite_) {
		if (selectedSprite_->GetName() == sprite->GetName()) {

			selectedSprite_ = nullptr;
		}
	}

	sprites_.erase(std::remove(sprites_.begin(), sprites_.end(), sprite), sprites_.end());
}

void SpriteRenderer::Clear() {

	sprites_.clear();
	selectedSprite_ = nullptr;
}

void SpriteRenderer::SelectSprite() {

	ImGui::Begin("Sprite");

	// 他のObjectが選択されていたら選択解除する
	if (ParticleRenderer::GetSelectedParticle() ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor() ||
		GameSystem::GameLight()->SelectedGameLight() ||
		GameSystem::GameCamera()->SelectedGameCamera()) {

		currentSpriteIndex_ = -1;
		selectedSprite_ = nullptr;
	}

	// UI 描画
	if (!sprites_.empty()) {
		for (int i = 0; i < sprites_.size(); ++i) {
			auto& sprite = sprites_[i];
			if (!sprite) {
				continue;
			}
			bool isSelected = (currentSpriteIndex_ == i);

			if (ImGui::Selectable(sprite->GetName().c_str(), isSelected)) {
				currentSpriteIndex_ = i;
				selectedSprite_ = sprite;
			}
		}
	}

	ImGui::End();

}

void SpriteRenderer::SelectedImGui() {

	if (!selectedSprite_) {
		return;
	}

	ImGui::Text(selectedSprite_->GetName().c_str());
	ImGui::Separator();
	selectedSprite_->ImGui();
}