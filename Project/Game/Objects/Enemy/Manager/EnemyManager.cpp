#include "EnemyManager.h"

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

//============================================================================*/
//	EnemyManager classMethods
//============================================================================*/

void EnemyManager::Init() {

	BaseEditor::SetEditor("enemyManager");

	enemyIndex_ = 0;

}

void EnemyManager::Update() {

	// 全ての敵の更新
	enemies_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		return !enemy->IsAlive(); });
	for (const auto& enemy : enemies_) {

		enemy->Update();
	}

}

void EnemyManager::ImGui() {

	if (ImGui::Button("add Enemy")) {

		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Init(enemyIndex_);

		enemies_.emplace_back(std::move(newEnemy));

		++enemyIndex_;
	}
}