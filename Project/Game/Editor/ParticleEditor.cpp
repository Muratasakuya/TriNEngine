#include "ParticleEditor.h"
#include "ParticleEditor.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	ParticleEditor classMethods
//============================================================================*/

ParticleEditor::ParticleEditor() {

	BaseEditor::SetEditor("particleEditor");

	addParticleType_ = ParticleType::kDispersion;

	selectedParticle_ = std::nullopt;

	addSelectedModelIndex_ = 0;
	addSelectedTextureIndex_ = 0;

}

void ParticleEditor::Update() {

	for (const auto& particle : particles_) {

		particle->Update();
	}
}

void ParticleEditor::ImGui() {

	//============================================================================*/
	//	Add Particle

	ImGui::Text("Add Particle");
	// particleの種類
	const char* particleOptions[] =
	{ "Dispersion", "Injection", "Chase" };
	int particleTypeIndex = static_cast<int>(addParticleType_);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 2.0f)); // 内部の余白調整
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));  // アイテム間隔調整

	// particleのType選択
	if (ImGui::BeginCombo("Type", particleOptions[particleTypeIndex])) {
		for (int index = 0; index < IM_ARRAYSIZE(particleOptions); index++) {

			const bool isSelected = (particleTypeIndex == index);
			if (ImGui::Selectable(particleOptions[index], isSelected)) {

				addParticleType_ = static_cast<ParticleType>(index);
				particleTypeIndex = index;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// modelの名前
	std::vector<std::string> modelKeys = Asset::GetModel()->GetModelKeys();

	// 使用するModelの選択
	if (ImGui::BeginCombo("Model", modelKeys[addSelectedModelIndex_].c_str())) {
		for (int index = 0; index < modelKeys.size(); ++index) {

			const bool isSelected = (addSelectedModelIndex_ == index);
			if (ImGui::Selectable(modelKeys[index].c_str(), isSelected)) {

				addSelectedModelIndex_ = index;
				addModelName_ = modelKeys[index];
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	// textureの名前
	std::vector<std::string> textureKeys = Asset::GetTexture()->GetTextureKeys();

	// 使用するTextureの選択
	if (ImGui::BeginCombo("Texture", textureKeys[addSelectedTextureIndex_].c_str())) {
		for (int index = 0; index < textureKeys.size(); ++index) {

			const bool isSelected = (addSelectedTextureIndex_ == index);
			if (ImGui::Selectable(textureKeys[index].c_str(), isSelected)) {

				addSelectedTextureIndex_ = index;
				addTextureName_ = textureKeys[index];
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	static char particleNameBuffer[128] = "";
	if (ImGui::InputText("Name", particleNameBuffer, IM_ARRAYSIZE(particleNameBuffer))) {

		addParticleName_ = particleNameBuffer;
	}

	ImGui::PopStyleVar(2);

	if (ImGui::Button("Add", ImVec2(144.0f, 20.0f))) {

		if (addModelName_ == "") {
			addModelName_ = "plane.obj";
		}
		if (addTextureName_ == "") {
			addTextureName_ = "circle";
		}
		if (addParticleName_ == "") {
			addParticleName_ = "demoParticle";
		}

		// 設定された内容でParticleを生成
		CreateAddParticle();
	}

	//============================================================================*/
	//	Edit Particle
	// ここで生成されたParticleのImGuiでの設定を行う

	std::unordered_map<std::string, std::vector<std::pair<int, IBaseParticle*>>> groupedParticles;
	auto GetBaseName = [](const std::string& name) {
		size_t index = name.find_last_not_of("0123456789");
		if (index != std::string::npos && index + 1 < name.size()) {
			return name.substr(0, index + 1); // 数字部分を除外した基本名を取得
		}
		return name; };

	for (int i = 0; i < particles_.size(); ++i) {
		if (particles_[i]) {

			IBaseParticle* baseParticle = particles_[i].get();
			if (baseParticle) {

				std::string baseName = GetBaseName(baseParticle->GetName());
				groupedParticles[baseName].emplace_back(i, baseParticle);
			}
		}
	}

	ImGui::Begin("ParticleList");

	// UI 描画
	// Particleの選択
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

	ImGui::Begin("ParticleEdit");

	if (selectedParticle_.has_value()) {
		ImGui::Text(selectedParticle_.value()->GetName().c_str());
		ImGui::Separator();
		selectedParticle_.value()->ImGui();
	}

	ImGui::End();

}

void ParticleEditor::CreateAddParticle() {

	// 名前の重複チェック
	std::string uniqueName = GetUniqueParticleName(addParticleName_, particles_);

	switch (addParticleType_) {
	case ParticleType::kDispersion:

		AddParticle<DispersionParticle>(uniqueName, addModelName_, addTextureName_);
		break;
	case ParticleType::kInjection:

		AddParticle<InjectionParticle>(uniqueName, addModelName_, addTextureName_);
		break;
	case ParticleType::kChase:

		AddParticle<ChaseParticle>(uniqueName, addModelName_, addTextureName_);
		break;
	default:
		throw std::runtime_error("Unknown particle type");
	}
}

std::string ParticleEditor::GetUniqueParticleName(
	const std::string& baseName, const std::vector<std::unique_ptr<IBaseParticle>>& particles) {

	int suffix = 0;
	std::string newName = baseName;

	auto isNameUsed = [&particles](const std::string& name) {
		for (const auto& particle : particles) {
			if (particle->GetName() == name) {
				return true;
			}
		}
		return false;
		};

	while (isNameUsed(newName)) {
		newName = baseName + std::to_string(++suffix);
	}
	return newName;
}
