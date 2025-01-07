#include "TitleScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Engine/Process/Input.h>
#include <Game/Scenes/Manager/SceneManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>

//============================================================================*/
//	TitleScene classMethods
//============================================================================*/

void TitleScene::LoadAssets() {

	//========================================================================*/
	//* textures

	Asset::LoadTexture("title");

}

void TitleScene::Init() {

	GameSystem::GameCamera()->Init();

	LoadAssets();

	titleSprite_ = std::make_unique<TitleSprite>();
	titleSprite_->Init();

}

void TitleScene::Update([[maybe_unused]] SceneManager* sceneManager) {

	titleSprite_->Update();

	// AボタンでGameSceneに遷移する
	if (Input::GetInstance()->TriggerGamepadButton(InputGamePadButtons::A)) {

		sceneManager->SetNextScene("Game");

		GameTimer::SetReturnScaleEnable(true);
	}

}