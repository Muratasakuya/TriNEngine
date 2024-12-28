#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/3D/Object/BaseAnimationObject.h>

// c++
#include <memory>
#include <optional>
#include <initializer_list>
#include <unordered_set>

class FollowCamera;

//============================================================================*/
//	Player class
//============================================================================*/
class Player :
	public BaseAnimationObject {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Player() = default;
	~Player() = default;

	void Init();

	void Update();

	//* imgui *//

	void DerivedImGui() override;

	//* getter *//

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structures

	// 補間
	struct LerpValue {

		// value
		float start;
		float end;
		float current;

		// timer
		float lerpTime;
		float lerpTimer;
	};

	// 移動
	enum class MoveBehaviour {

		Dash, // ダッシュ
		Jump, // ジャンプ
	};

	//========================================================================*/
	//* variables

	Input* input_ = nullptr;
	FollowCamera* followCamera_ = nullptr;

	std::optional<MoveBehaviour> moveBehaviour_;              //* 依頼移動ビヘイビア
	std::unordered_set<MoveBehaviour> currentMoveBehaviours_; //* 現在の移動ビヘイビア

	std::unordered_map<std::string, std::string> animationNames_;
	std::string currentAnimationKey_;

	//* base *//

	Vector3 velocity_; //* 移動速度
	Vector3 move_;     //* 移動量

	float rotationLerpRate_; //* 回転補間割合
	float moveDecay_;        //* 速度減衰率

	//* dash *//

	LerpValue dashSpeed_; //* ダッシュ速度

	//* jump *//

	float jumpStrength; //* ジャンプ力

	//* bool *//

	bool isDashing_;  //* ダッシュしたかどうか
	bool isOnGround_; //* 地面に着いているかどうか

	//========================================================================*/
	//* functions

	//====================================*/
	//* move

	void Move();        //* 全体の移動を管理
	void MoveRequest(); //* 移動依頼

	void MoveWalk(); //* 通常移動、歩きに該当する
	void MoveDash(); //* ダッシュ、Rを押している間はダッシュ
	void MoveJump(); //* ジャンプ

	//====================================*/
	//* attack

	//====================================*/
	//* other

	// json
	void ApplyJson();
	void SaveJson();

	// helper
	void RotateToDirection();
	bool CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states);

};