#include "GameScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	GameScene classMethods
//============================================================================*/

void GameScene::LoadAssets() {

	Asset::LoadTexture("fieldBaseTile");

}

void GameScene::Init() {

	LoadAssets();

	field_ = std::make_unique<Field>();
	field_->Init();

}

void GameScene::Update([[maybe_unused]] SceneManager* sceneManager) {

	field_->Update();

}