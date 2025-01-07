#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Game/Scenes/Manager/SceneManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>
#include <Lib/Adapter/Easing.h>

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

	// particle
	Asset::LoadTexture("circle");
	Asset::LoadTexture("starCircle");

	//========================================================================*/
	//* models

	// environment
	Asset::LoadModel("./Resources/Model/Obj/Environment", "wall.obj");

	// player
	Asset::LoadModel("./Resources/Model/Gltf/", "player.gltf");
	Asset::LoadAnimation("./Resources/Model/Gltf/", "player.gltf", "player.gltf");

	// enemy
	Asset::LoadModel("./Resources/Model/Obj/Enemy", "slime.obj");

	// particle
	Asset::LoadModel("./Resources/Model/Obj/CG", "plane.obj");
	Asset::LoadModel("./Resources/Model/Obj/Particle", "hitLine.obj");

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

	player_->SetEnemyManager(enemyManager_.get());

	timeLimit_ = std::make_unique<TimeLimit>();
	timeLimit_->Init(enemyManager_.get());

	GameSystem::GameCamera()->GetFollowCamera()->SetTarget(&player_->GetWorldTransform());

	//* timeScale *//


	finishScaleTimer_ = 0.0f;
	finishScaleTime_ = 0.8f;

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
	if (timeLimit_->IsFinish() || enemyManager_->IsFinish()) {

		finishScaleTimer_ += GameTimer::GetDeltaTime();
		float t = finishScaleTimer_ / finishScaleTime_;
		float easedT = EaseOutExpo(t);

		if (finishScaleTime_ > finishScaleTimer_) {
			GameTimer::SetTimeScale(std::lerp(1.0f, 0.08f, easedT));
			GameTimer::SetReturnScaleEnable(false);
		}

		sceneManager->SetNextScene("Title");
	}

}