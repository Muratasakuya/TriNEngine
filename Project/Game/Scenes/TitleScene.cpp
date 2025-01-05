#include "TitleScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Engine/Process/Input.h>
#include <Game/Scenes/Manager/SceneManager.h>

//============================================================================*/
//	TitleScene classMethods
//============================================================================*/

void TitleScene::LoadAssets() {

	//========================================================================*/
	//* textures

	Asset::LoadTexture("title");

}

void TitleScene::Init() {

	LoadAssets();

	titleSprite_ = std::make_unique<TitleSprite>();
	titleSprite_->Init();

}

void TitleScene::Update([[maybe_unused]] SceneManager* sceneManager) {

	titleSprite_->Update();

	// AボタンでGameSceneに遷移する
	if (Input::GetInstance()->TriggerGamepadButton(InputGamePadButtons::A)) {

		sceneManager->SetNextScene("Game");
	}

}