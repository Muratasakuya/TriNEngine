#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Base/BaseSprite.h>
#include <Game/Editor/Base/BaseEditor.h>

// front
class EnemyManager;

//============================================================================*/
//	TimeNumber class
//============================================================================*/
class TimeNumber :
	public BaseSprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TimeNumber() = default;
	~TimeNumber() = default;

	void Init(uint32_t index);

};
//============================================================================*/
//	TimeCoron class
//============================================================================*/
class TimeCoron :
	public BaseSprite {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TimeCoron() = default;
	~TimeCoron() = default;

	void Init();

};

//============================================================================*/
//	TimeLimit class
//============================================================================*/
class TimeLimit :
	public BaseEditor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TimeLimit() = default;
	~TimeLimit() = default;

	void Init(EnemyManager* enemyManager);

	void Update();

	void ImGui() override;

	//* getter *//

	bool IsFinish() const { return isFinish_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	EnemyManager* enemyManager_ = nullptr;

	int time_;      //* 進める時間
	int timeLimit_; //* 制限時間

	float waitTimer_;
	float waitTime_;

	float accumulatedTime_;

	bool isCountStart_;
	bool isFinish_;

	std::vector<std::unique_ptr<TimeNumber>> timeNumbers_;
	std::vector<Vector2> timeNumberPositions_;

	std::unique_ptr<TimeCoron> timeCoron_;
	Vector2 timeCoronPos_;

	//===================================================================*/
	///* functions

	void ApplyJson();
	void SaveJson();

};