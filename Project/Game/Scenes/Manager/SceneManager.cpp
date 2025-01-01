#include "SceneManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	SceneManager classMethods
//============================================================================*/

SceneManager::SceneManager(const std::string& firstSceneName) {

	isSceneSwitching_ = false;
	gameLoop_ = true;

	LoadScene(firstSceneName);

	currentScene_->Init();

}

void SceneManager::Update() {

	if (isSceneSwitching_) {

		currentScene_->Init();
		isSceneSwitching_ = false;
	}

	currentScene_->Update();

}

void SceneManager::SwitchScene() {

	if (isSceneSwitching_) {

		LoadScene(nextSceneName_);
	}
}

void SceneManager::SetNextScene(const std::string& sceneName) {

	nextSceneName_ = sceneName;
	isSceneSwitching_ = true;
}

void SceneManager::Finalize() {
	currentScene_.reset();
}

void SceneManager::LoadScene(const std::string& sceneName) {

	currentScene_.reset();

	//* CreateNewScene *//
	currentScene_ = SceneFactory::CreateScene(sceneName);
}

void SceneManager::ImGui() {
#ifdef _DEBUG

	ImGui::SetCursorPos(ImVec2(397.0f, 64.0f + 432.0f + 4.0f));
	ImGui::BeginChild("SceneChild",
		ImVec2((((static_cast<float>(kWindowWidth) / 2.0f) + (768.0f / 2.0f) - 254.0f)) / 2.0f, 212.0f),
		true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("Scene");
	ImGui::Separator();
	std::string currentSceneText = "CurrentScene: " + currentScene_->GetSceneName();
	ImGui::Text("%s", currentSceneText.c_str());

	std::vector<std::string> sceneList = { "Title", "Game" };
	sceneList.erase(std::remove(sceneList.begin(), sceneList.end(), currentScene_->GetSceneName()), sceneList.end());

	ImGui::SetNextItemWidth(162.0f);
	if (ImGui::BeginCombo("TransitionTarget", sceneList[selectedSceneIndex_].c_str(), ImGuiComboFlags_NoArrowButton)) {
		for (int i = 0; i < sceneList.size(); ++i) {
			bool isSelected = (selectedSceneIndex_ == i);
			if (ImGui::Selectable(sceneList[i].c_str(), isSelected)) {

				selectedSceneIndex_ = i;

				SetNextScene(sceneList[selectedSceneIndex_]);

			}
			if (isSelected) {

				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::EndChild();

#endif // _DEBUG
}