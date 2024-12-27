#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

// c++
#include <string>
#include <memory>
#include <cassert>

//============================================================================*/
//	SceneFactory class
//============================================================================*/
class SceneFactory {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SceneFactory() = default;
	~SceneFactory() = default;

	static std::unique_ptr<IScene> CreateScene(const std::string& sceneName);

};