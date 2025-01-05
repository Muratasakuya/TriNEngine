#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	GameScene classMethods
//============================================================================*/

void GameScene::LoadAssets() {

	//========================================================================*/
	//* textures

	Asset::LoadTexture("white");

	// timer
	Asset::LoadTexture("timeNumber");
	Asset::LoadTexture("coron");

	// environment
	Asset::LoadTexture("fieldBaseTile");
	Asset::LoadTexture("wall");

	//========================================================================*/
	//* models

	// environment
	Asset::LoadModel("./Resources/Model/Obj/Environment", "wall.obj");

	// player
	Asset::LoadModel("./Resources/Model/Gltf/", "player.gltf");
	Asset::LoadAnimation("./Resources/Model/Gltf/", "player.gltf", "player.gltf");

	// enemy
	Asset::LoadModel("./Resources/Model/Obj/Enemy", "slime.obj");

}

void GameScene::Init() {

	LoadAssets();

	field_ = std::make_unique<Field>();
	field_->Init();

	player_ = std::make_unique<Player>();
	player_->Init();

	const size_t wallNum = 4;
	walls_.resize(wallNum);
	for (uint32_t index = 0; index < wallNum; ++index) {

		walls_[index] = std::make_unique<Wall>();
		walls_[index]->Init(player_.get(), index);
	}

	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Init(player_.get());

	timeLimit_ = std::make_unique<TimeLimit>();
	timeLimit_->Init();

	GameSystem::GameCamera()->GetFollowCamera()->SetTarget(&player_->GetWorldTransform());

}

void GameScene::Update([[maybe_unused]] SceneManager* sceneManager) {

	GameSystem::GameCamera()->GetSunLightCamera()->SetTranslate(player_->GetWorldPos());

	field_->Update();
	for (const auto& wall : walls_) {
		wall->Update();
	}

	player_->Update();

	enemyManager_->Update();

	timeLimit_->Update();

}