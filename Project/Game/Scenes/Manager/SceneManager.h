#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>
#include <Game/Scenes/Methods/SceneFactory.h>

// c++
#include <string>
#include <memory>

//============================================================================*/
//	SceneManager class
//============================================================================*/
class SceneManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SceneManager(const std::string& firstSceneName);
	~SceneManager() = default;

	void Update();

	void SwitchScene();

	void SetNextScene(const std::string& sceneName);

	//* imgui *//

	void ImGui();

	//* getter *//

	bool IsSceneSwitching() const { return isSceneSwitching_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<IScene> currentScene_;

	std::string nextSceneName_;
	bool isSceneSwitching_;
	bool gameLoop_;

	int selectedSceneIndex_;

	//===================================================================*/
	///* functions

	void LoadScene(const std::string& sceneName);

};