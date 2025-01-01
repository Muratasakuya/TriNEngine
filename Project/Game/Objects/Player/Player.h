#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/3D/Object/BaseAnimationObject.h>

// Collider
#include <Game/Objects/Player/Collision/PlayerAttackCollider.h>

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

	void Draw(RendererPipelineType pipeline) override;

	//* imgui *//

	void DerivedImGui() override;

	//* getter *//

	bool IsWaitToFirstAttack() const { return isWaitToFirstAttack_; }

	bool IsWaitToSecondAttack() const { return isWaitToSecondAttackEnable_; }

	bool IsWaitToThirdAttack() const { return isWaitToThirdAttackEnable_; }

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

		// 移動
		Dash,              // ダッシュ
		Jump,              // ジャンプ

		// 攻撃
		WaitToFirstAttack,  // 待機からの最初の攻撃
		WaitToSecondAttack, // 最初の攻撃からの2回目の攻撃
		WaitToThirdAttack,  // 2回目の攻撃からの3回目の攻撃
	};

	//========================================================================*/
	//* variables

	Input* input_ = nullptr;
	FollowCamera* followCamera_ = nullptr;

	std::unique_ptr<PlayerAttackCollider> attackCollider_;

	std::optional<MoveBehaviour> moveBehaviour_;              //* 依頼移動ビヘイビア
	std::unordered_set<MoveBehaviour> currentMoveBehaviours_; //* 現在の移動ビヘイビア

	std::unordered_map<std::string, std::string> animationNames_;
	std::string currentAnimationKey_;

	// 移動
	float waitToDashDuration_;   //* 待機         -> ダッシュ
	float moveToJumpDuration_;   //* 何かしらの動き -> ジャンプ
	float moveToWaitDuration_;   //* 何かしらの動き -> 待機

	// 攻撃
	float waitToAttackDuration_;       //* 待機       -> 最初の攻撃
	float waitToSecondAttackDuration_; //* 最初の攻撃  -> 2回目の攻撃
	float waitToThirdAttackDuration_;  //* 2回目の攻撃 -> 3回目の攻撃

	//* base *//

	Vector3 velocity_; //* 移動速度
	Vector3 move_;     //* 移動量

	float rotationLerpRate_; //* 回転補間割合
	float moveDecay_;        //* 速度減衰率

	//* dash *//

	LerpValue dashSpeed_; //* ダッシュ速度

	//* bool *//

	bool isDrawDebugCollider_; //* 攻撃用Colliderの描画有無

	bool isDashing_;           //* ダッシュしたかどうか
	bool isJump_;              //* ジャンプしたかどうか

	bool isWaitToFirstAttack_;        //* 待機からの最初の攻撃をしたかどうか
	bool isWaitToSecondAttackEnable_; //* 二回目の攻撃を行うかどうか
	bool isWaitToThirdAttackEnable_;  //* 三回目の攻撃を行うかどうか

	//========================================================================*/
	//* functions

	//====================================*/
	//* move

	void UpdateAnimation(); //* アニメーション更新

	void Move();        //* 全体の移動を管理
	void MoveRequest(); //* 移動依頼
	void AttackRequest(); //* 攻撃依頼

	void MoveWalk(); //* 通常移動、歩きに該当する
	void MoveDash(); //* ダッシュ、Rを押している間はダッシュ
	void MoveJump(); //* ジャンプ

	//====================================*/
	//* attack

	void WaitToFirstAttack(); //* 待機からの最初の攻撃

	//====================================*/
	//* other

	// json
	void ApplyJson();
	void SaveJson();

	// helper
	void RotateToDirection();
	bool CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states);

};