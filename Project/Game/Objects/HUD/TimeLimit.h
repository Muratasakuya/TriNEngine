#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/2D/Base/BaseSprite.h>
#include <Game/Editor/Base/BaseEditor.h>

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

	void Init();

	void Update();

	void ImGui() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	int time_;      //* 進める時間
	int timeLimit_; //* 制限時間

	std::vector<std::unique_ptr<TimeNumber>> timeNumbers_;
	std::vector<Vector2> timeNumberPositions_;

	std::unique_ptr<TimeCoron> timeCoron_;
	Vector2 timeCoronPos_;

	//===================================================================*/
	///* functions

	void ApplyJson();
	void SaveJson();

};