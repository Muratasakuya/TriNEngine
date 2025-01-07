#include "ParticleRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	ParticleRenderer classMethods
//============================================================================*/

std::vector<IBaseParticle*> ParticleRenderer::particles_ = {};
IBaseParticle* ParticleRenderer::selectedParticle_ = nullptr;
int ParticleRenderer::currentParticleIndex_ = 0;

void ParticleRenderer::Render() {

	for (const auto& particle : particles_) {

		particle->Draw();
	}
}

void ParticleRenderer::SetParticle(IBaseParticle* particle) {

	if (!particle) {
		return;
	}

	particles_.emplace_back(particle);
}

void ParticleRenderer::EraseParticle(IBaseParticle* particle) {

	if (selectedParticle_) {
		if (selectedParticle_->GetName() == particle->GetName()) {

			selectedParticle_ = nullptr;
		}
	}

	particles_.erase(std::remove(particles_.begin(), particles_.end(), particle), particles_.end());
}

void ParticleRenderer::Clear() {

	particles_.clear();
	selectedParticle_ = nullptr;
}

void ParticleRenderer::SelectParticle() {

	ImGui::Begin("Particle");

	// 他のObjectが選択されていたら選択解除する
	if (SpriteRenderer::GetSelectedSprite() ||
		MeshRenderer::GetSelectedObject() ||
		EditorManager::GetSelectedEditor() ||
		GameSystem::GameLight()->SelectedGameLight() ||
		GameSystem::GameCamera()->SelectedGameCamera()) {

		currentParticleIndex_ = -1;
		selectedParticle_ = nullptr;
	}

	// 名前ごとにグループ化するためのマップ
	std::unordered_map<std::string, std::vector<std::pair<int, IBaseParticle*>>> groupedParticles;

	auto GetBaseName = [](const std::string& name) {
		size_t index = name.find_last_not_of("0123456789");
		if (index != std::string::npos && index + 1 < name.size()) {
			return name.substr(0, index + 1); // 数字部分を除外した基本名を取得
		}
		return name;
		};

	for (int i = 0; i < particles_.size(); ++i) {
		if (particles_[i]) {
			std::string baseName = GetBaseName(particles_[i]->GetName());
			groupedParticles[baseName].emplace_back(i, particles_[i]);
		}
	}

	// UI 描画
	if (!groupedParticles.empty()) {
		for (const auto& group : groupedParticles) {

			const std::string& baseName = group.first;
			const auto& particles = group.second;

			if (particles.size() == 1) {
				int index = particles[0].first;
				auto& particle = particles[0].second;

				if (ImGui::Selectable(particle->GetName().c_str(), currentParticleIndex_ == index)) {

					currentParticleIndex_ = index;
					selectedParticle_ = particle;
				}
			} else {
				// 2つ以上の場合はListで表示
				if (ImGui::TreeNode((baseName + "List").c_str())) {
					for (const auto& item : particles) {

						int index = item.first;
						auto& particle = item.second;

						if (ImGui::Selectable(particle->GetName().c_str(), currentParticleIndex_ == index)) {

							currentParticleIndex_ = index;
							selectedParticle_ = particle;
						}
					}
					ImGui::TreePop();
				}
			}
		}
	}

	ImGui::End();
}

void ParticleRenderer::SelectedImGui() {

	if (!selectedParticle_) {
		return;
	}

	ImGui::Text(selectedParticle_->GetName().c_str());
	ImGui::Separator();
	selectedParticle_->ImGui();
}