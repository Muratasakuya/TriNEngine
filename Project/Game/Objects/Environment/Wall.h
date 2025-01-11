#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>
#include <Game/Utility/Direction.h>

// c++
#include <chrono>

// front
class Player;

//============================================================================*/
//	Wall class
//============================================================================*/
class Wall :
	public BaseGameObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Wall() = default;
	~Wall() = default;

	void Init(Player* player, uint32_t index);

	void Update();

	//* imgui *//

	void DerivedImGui() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Player* player_ = nullptr;

	std::chrono::time_point<std::chrono::steady_clock> startTime_;

	//========================================================================*/
	//* functions

	void ApplyJson() override;
	void SaveJson() override;

};