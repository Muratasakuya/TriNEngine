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

	Asset::LoadTexture("field");
	Asset::LoadTexture("white");

	//========================================================================*/
	//* models

	Asset::LoadModel("./Resources/Model/Obj/CG", "cube.obj");

	Asset::LoadModel("./Resources/Model/Gltf/", "player.gltf");
	Asset::LoadAnimation("./Resources/Model/Gltf/", "player.gltf","player.gltf");

}

void GameScene::Init() {

	LoadAssets();

	for (uint32_t index = 0; index < 3; ++index) {

		cubes_.emplace_back();
		cubes_[index] = std::make_unique<Cube>();
		cubes_[index]->Init(index);
	}

	field_ = std::make_unique<Field>();
	field_->Init();

	player_ = std::make_unique<Player>();
	player_->Init();

	GameSystem::GameCamera()->GetFollowCamera()->SetTarget(&player_->GetWorldTransform());

}

void GameScene::Update() {

	GameSystem::GameCamera()->GetSunLightCamera()->SetTranslate(player_->GetWorldPos());

	for (uint32_t index = 0; index < 3; ++index) {

		cubes_[index]->Update();
	}

	field_->Update();

	player_->Update();

}