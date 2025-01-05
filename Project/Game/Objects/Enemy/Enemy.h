#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Object/BaseGameObject.h>
#include <Game/3D/Collision/Collider.h>

// front
class Player;

//============================================================================*/
//	Enemy class
//============================================================================*/
class Enemy :
	public BaseGameObject, Collider {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Enemy() = default;
	~Enemy() = default;

	void Init(uint32_t index, const Vector3& translate, Player* player);

	void Update();

	void Draw(RendererPipelineType pipeline) override;

	//* collision *//

	void OnCollisionEnter([[maybe_unused]] Collider* other) override;

	//* getter *//

	bool IsAlive() const { return isAlive_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Player* player_ = nullptr;

	const float initPosY_ = 2.0f;   // 初期Y座標
	const float initScaleY_ = 0.2f; // 初期Yスケール

	Vector3 velocity_;
	Vector3 moveVelocity_;
	Vector3 hitDirection_;

	//* life *//

	bool isHitDamage_;   //* ダメージを受けたか
	bool isSetVelocity_; //* 初速を与えるフラグ
	bool isAlive_;       //* 生存フラグ

	float aliveTime_;  //* 消えるまでの時間管理
	float aliveTimer_; //* 消えるまでの時間

	float rotateValue_; //* 消えるときの回転

	//* 動くときのクールタイム
	float moveCoolTimer_;
	float moveCoolTime_;

	const int maxHp_ = 3;
	int currentHp_;

	// 最初の落ちてくるAnimation
	float startAnimationTimer_;
	float startAnimationTime_;

	bool isAnimationFinish_;

	//========================================================================*/
	//* functions

	void StartAnimation();

	void KnockbackAnimation();

	void DeadAnimation();

	void Move();

	void RotateToDirection();

};