#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

// object
#include <Game/Objects/Environment/Field.h>
#include <Game/Objects/Environment/Wall.h>
#include <Game/Objects/Player/Player.h>
#include <Game/Objects/Enemy/Manager/EnemyManager.h>

// particle
#include <Game/Objects/Test/TestParticle.h>

// HUD
#include <Game/Objects/HUD/TimeLimit.h>

// c++
#include <string>
#include <memory>

//============================================================================*/
//	GameScene class
//============================================================================*/
class GameScene :
	public IScene {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GameScene() = default;
	~GameScene() = default;

	void Init() override;

	void Update([[maybe_unused]] SceneManager* sceneManager) override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<Field> field_;
	std::vector<std::unique_ptr<Wall>> walls_;

	std::unique_ptr<Player> player_;

	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<TimeLimit> timeLimit_;

	//* timeScale *//

	float finishScaleTimer_;
	float finishScaleTime_;

	//========================================================================*/
	//* function

	void LoadAssets();

};