#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Editor/Base/BaseEditor.h>
#include <Game/Objects/Enemy/Enemy.h>

// c++
#include <memory>
#include <list>
#include <unordered_map>

// front
class Player;

//============================================================================*/
//	EnemyManager class
//============================================================================*/
class EnemyManager :
	public BaseEditor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	EnemyManager() = default;
	~EnemyManager() = default;

	void Init(Player* player);

	void Update();

	void ImGui() override;

	//* getter *//

	bool IsStart() const { return isStart_; }
	bool IsFinish() const { return isFinish_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* enum

	enum class SpawnPlace {

		Center,     //* 中心
		FrontRight, //* 右前 (x+,z+)
		BackRight,  //* 右後 (x+,z-)
		FrontLeft,  //* 左前 (x-,z+)
		BackLeft    //* 左後 (x-,z-)
	};

	//========================================================================*/
	//* variables

	Player* player_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemies_;
	uint32_t enemyIndex_;

	bool isStart_;
	bool isFinish_;

	// キーごとの敵の出現座標
	std::unordered_map<SpawnPlace, std::vector<Vector3>> spawnPositions_;

	uint32_t phase_;
	std::optional<uint32_t> phaseController_;

	//* editor *//

	Vector3 editSpawnPos_;

	SpawnPlace selectedPlace_;

	//* test *//

	float debugStartTimer_;

	//========================================================================*/
	//* functions

	void UpdatePhase();

	void ApplyJson();
	void SaveJson();

};