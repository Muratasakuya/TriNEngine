#include "SceneFactory.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/GameScene.h>
#include <Game/Scenes/TitleScene.h>

//============================================================================*/
//	SceneFactory classMethods
//============================================================================*/

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName) {

	if (sceneName == "Game") {

		return std::make_unique<GameScene>();
	} else if (sceneName == "Title") {

		return std::make_unique<TitleScene>();
	}

	assert(false && "input scene Name does not Create Methods");
	return nullptr;
}