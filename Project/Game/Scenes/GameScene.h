#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Scenes/Methods/IScene.h>

// object
#include <Game/Objects/Test/Cube.h>
#include <Game/Objects/Environment/Field.h>
#include <Game/Objects/Player/Player.h>

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

	void Update() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::vector<std::unique_ptr<Cube>> cubes_;

	std::unique_ptr<Field> field_;

	std::unique_ptr<Player> player_;

	//========================================================================*/
	//* function

	void LoadAssets();

};