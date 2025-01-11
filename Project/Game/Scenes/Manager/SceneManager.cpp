#include "SceneManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Asset/Asset.h>
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	SceneManager classMethods
//============================================================================*/

SceneManager::SceneManager(const std::string& firstSceneName) {

	isSceneSwitching_ = false;
	gameLoop_ = true;

	LoadScene(firstSceneName);

	currentScene_->Init();

	Asset::LoadTexture("transitionWhite");

	sceneTransition_ = std::make_unique<SceneTransition>();
	sceneTransition_->Init();

#ifdef _DEBUG
	demoEditorScene_ = std::make_unique<DemoEditorScene>();
	demoEditorScene_->Init();
#endif // _DEBUG

}

void SceneManager::Update() {

	currentScene_->Update(this);

	sceneTransition_->Update();

#ifdef _DEBUG
	demoEditorScene_->Update();
#endif // _DEBUG

}

void SceneManager::SwitchScene() {

	if (sceneTransition_->IsBeginTransitionFinished()) {

		isSceneSwitching_ = true;
		sceneTransition_->SetResetBeginTransition();
	}

	if (isSceneSwitching_) {

		LoadScene(nextSceneName_);
	}
}

void SceneManager::InitNextScene() {

	if (isSceneSwitching_) {

		currentScene_->Init();
		isSceneSwitching_ = false;
	}
}

void SceneManager::SetNextScene(const std::string& sceneName) {

	nextSceneName_ = sceneName;

	sceneTransition_->SetTransition();
}

void SceneManager::Finalize() {
	currentScene_.reset();
#ifdef _DEBUG
	demoEditorScene_.reset();
#endif // _DEBUG
}

void SceneManager::LoadScene(const std::string& sceneName) {

	currentScene_.reset();

	//* CreateNewScene *//
	currentScene_ = SceneFactory::CreateScene(sceneName);
}