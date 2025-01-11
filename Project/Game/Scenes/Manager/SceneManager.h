#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>
#include <Game/Scenes/Methods/SceneFactory.h>
#include <Game/Scenes/Methods/SceneTransition.h>
#include <Game/Scenes/Editor/DemoEditorScene.h>

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

	void InitNextScene();

	void SetNextScene(const std::string& sceneName);

	void Finalize();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<IScene> currentScene_;
	std::unique_ptr<DemoEditorScene> demoEditorScene_;

	std::unique_ptr<SceneTransition> sceneTransition_;

	std::string nextSceneName_;
	bool isSceneSwitching_;
	bool gameLoop_;

	int selectedSceneIndex_;

	//===================================================================*/
	///* functions

	void LoadScene(const std::string& sceneName);

};