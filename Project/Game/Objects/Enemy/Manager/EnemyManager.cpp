#include "EnemyManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/JsonAdapter.h>
#include <Game/Utility/GameTimer.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	EnemyManager classMethods
//============================================================================*/

void EnemyManager::Init(Player* player) {

	player_ = player;

	debugStartTimer_ = 0.0f;

	BaseEditor::SetEditor("enemyManager");

	enemyIndex_ = 0;
	selectedPlace_ = SpawnPlace::Center;

	ApplyJson();

	phase_ = 0;
	phaseController_ = std::nullopt;

	isStart_ = false;
	isFinish_ = false;

	deadParticles_.resize(8);
	for (uint32_t index = 0; index < 8; ++index) {

		deadParticles_[index] = std::make_unique<EnemyDeadParticle>(index);
		deadParticles_[index]->Init();
	}

}

void EnemyManager::Update() {

	debugStartTimer_ += GameTimer::GetDeltaTime();
	if (debugStartTimer_ >= 2.0f) {
		UpdatePhase();
	}

	int index = 0;
	for (const auto& enemy : enemies_) {
		if (!enemy->IsAlive()) {

			deadParticles_[index]->EmitOnce(enemy->GetTranslation());
			++index;
		}
	}

	// 全ての敵の更新
	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		return !enemy->IsAlive(); });

	if (phaseController_.has_value() && phaseController_.value() == 3) {
		if (enemies_.size() == 1) {
			if (enemies_.front()->GetHp() == 0) {

				isFinish_ = true;
			}
		}
	}

	for (const auto& enemy : enemies_) {

		enemy->Update();
	}

	for (const auto& deadParticle : deadParticles_) {

		deadParticle->Update();
	}

}

void EnemyManager::UpdatePhase() {

	isStart_ = true;

	// 最初のPhaseから次のPhaseへの処理
	if (phaseController_.has_value() && phaseController_.value() == 0) {
		// 敵の数が0になったら
		if (enemies_.size() == 0) {

			// 次のPhaseに進める
			phase_ = 1;
		}
	}

	// 2回目のPhaseから次のPhaseへの処理
	if (phaseController_.has_value() && phaseController_.value() == 1) {
		// 敵の数が0になったら
		if (enemies_.size() == 0) {

			// 次のPhaseに進める
			phase_ = 2;
		}
	}

	// 3回目のPhaseから次のPhase? への処理
	if (phaseController_.has_value() && phaseController_.value() == 2) {
		// 敵の数が0になったら
		if (enemies_.size() == 0) {

			// 最後のPhaseに進める
			phase_ = 3;
		}
	}

	// 最初の敵の発生処理
	if (phase_ == 0 && !phaseController_.has_value()) {
		// 中心
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::Center].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::Center][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 左前
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::FrontLeft].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::FrontLeft][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 右前
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::FrontRight].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::FrontRight][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}

		// 敵の発生処理終了
		phaseController_ = phase_;
	}

	// 2回目の敵の発生処理
	if (phase_ == 1 && phaseController_.value() == 0) {

		// 中心
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::Center].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::Center][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 左後ろ
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::BackLeft].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::BackLeft][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 右後ろ
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::BackRight].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::BackRight][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}

		// 敵の発生処理終了
		phaseController_ = phase_;
	}

	// 3回目の敵の発生処理
	if (phase_ == 2 && phaseController_.value() == 1) {

		// 中心
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::Center].size(); ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			newEnemy->Init(enemyIndex_, spawnPositions_[SpawnPlace::Center][index], player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 左前
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::FrontLeft].size() - 2; ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			Vector3 newSpawnPos = spawnPositions_[SpawnPlace::FrontLeft][index];
			newSpawnPos.z = 0.0f;
			newEnemy->Init(enemyIndex_, newSpawnPos, player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}
		// 右前
		for (uint32_t index = 0; index < spawnPositions_[SpawnPlace::FrontRight].size() - 2; ++index) {

			std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
			Vector3 newSpawnPos = spawnPositions_[SpawnPlace::FrontRight][index];
			newSpawnPos.z = 0.0f;
			newEnemy->Init(enemyIndex_, newSpawnPos, player_);

			enemies_.emplace_back(std::move(newEnemy));
			++enemyIndex_;
		}

		// 敵の発生処理終了
		phaseController_ = phase_;
	}

	//最後(仮)の敵の発生処理
	if (phase_ == 3 && phaseController_.value() == 2) {

		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Init(enemyIndex_, Vector3(0.0f, 0.0f, 0.0f), player_);

		enemies_.emplace_back(std::move(newEnemy));
		++enemyIndex_;

		// 敵の発生処理終了
		phaseController_ = phase_;
	}
}

void EnemyManager::ImGui() {

	if (ImGui::Button("Save")) {

		SaveJson();
	}

	ImGui::Separator();

	// 出現座標キーリスト
	const char* spawnPlaceNames[] = { "Center", "FrontRight", "BackRight", "FrontLeft", "BackLeft" };
	// 現在選択している場所
	int currentPlace = static_cast<int>(selectedPlace_);
	if (ImGui::Combo("Spawn Place", &currentPlace, spawnPlaceNames, IM_ARRAYSIZE(spawnPlaceNames))) {
		selectedPlace_ = static_cast<SpawnPlace>(currentPlace);
	}

	ImGui::DragFloat3(" spawnPos", &editSpawnPos_.x, 0.01f);

	if (ImGui::CollapsingHeader("Spawn Positions Preview")) {
		for (auto& [place, positions] : spawnPositions_) {
			std::string headerName = spawnPlaceNames[static_cast<int>(place)];
			if (ImGui::TreeNode(headerName.c_str())) {
				for (size_t i = 0; i < positions.size(); ++i) {
					ImGui::Text("Pos %zu: (%.2f, %.2f, %.2f)", i, positions[i].x, positions[i].y, positions[i].z);
					ImGui::SameLine();
					if (ImGui::Button(("Delete##" + std::to_string(i) + headerName).c_str())) {
						positions.erase(positions.begin() + i);
						break;
					}
				}
				ImGui::TreePop();
			}
		}
	}

	if (ImGui::Button("Add to Spawn Positions")) {
		spawnPositions_[selectedPlace_].push_back(editSpawnPos_);
	}

	ImGui::Separator();

	if (ImGui::Button("Add Enemy")) {
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Init(enemyIndex_, editSpawnPos_, player_);

		enemies_.emplace_back(std::move(newEnemy));
		++enemyIndex_;
	}

	static int selectedEnemyIndex = -1; // 削除対象の選択
	if (ImGui::CollapsingHeader("Enemy List")) {
		for (int index = 0; index < enemies_.size();) {

			ImGui::RadioButton(("Enemy " + std::to_string(index)).c_str(), &selectedEnemyIndex, index);
			++index;
		}
	}

	if (ImGui::Button("Delete Selected Enemy")) {
		if (selectedEnemyIndex >= 0 && selectedEnemyIndex < static_cast<int>(enemies_.size())) {

			auto it = enemies_.begin();
			std::advance(it, selectedEnemyIndex);
			enemies_.erase(it);
			selectedEnemyIndex = -1;
			--enemyIndex_;
		}
	}

	if (ImGui::Button("Clear Enemy")) {
		enemies_.clear();
	}

}

void EnemyManager::ApplyJson() {

	Json data = JsonAdapter::Load("Enemy/enemyEditor.json");

	auto stringToSpawnPlace = [](const std::string& str) -> std::optional<SpawnPlace> {
		if (str == "Center") return SpawnPlace::Center;
		if (str == "FrontRight") return SpawnPlace::FrontRight;
		if (str == "BackRight") return SpawnPlace::BackRight;
		if (str == "FrontLeft") return SpawnPlace::FrontLeft;
		if (str == "BackLeft") return SpawnPlace::BackLeft;
		return std::nullopt; };
	spawnPositions_.clear();

	for (const auto& [key, value] : data.items()) {
		auto placeOpt = stringToSpawnPlace(key);
		if (!placeOpt.has_value()) {
			continue;
		}

		SpawnPlace place = placeOpt.value();
		std::vector<Vector3> positions;

		for (const auto& posData : value) {

			Vector3 pos = JsonAdapter::ToVector3(posData);
			positions.push_back(pos);
		}

		// spawnPositions_に追加
		spawnPositions_[place] = positions;
	}

}

void EnemyManager::SaveJson() {

	Json data;

	auto spawnPlaceToString = [](SpawnPlace place) -> std::string {
		switch (place) {
		case SpawnPlace::Center:     return "Center";
		case SpawnPlace::FrontRight: return "FrontRight";
		case SpawnPlace::BackRight:  return "BackRight";
		case SpawnPlace::FrontLeft:  return "FrontLeft";
		case SpawnPlace::BackLeft:   return "BackLeft";
		default:                     return "Unknown";
		}};

	for (const auto& [place, positions] : spawnPositions_) {
		Json positionArray = Json::array();

		for (const auto& pos : positions) {
			positionArray.push_back(JsonAdapter::FromVector3(pos));
		}
		data[spawnPlaceToString(place)] = positionArray;
	}

	JsonAdapter::Save("Enemy/enemyEditor.jdon", data);

}