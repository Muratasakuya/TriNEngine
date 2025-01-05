#include "Enemy.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Game/Objects/Player/Player.h>
#include <Game/Utility/GameTimer.h>
#include <Game/Utility/Direction.h>
#include <Lib/Adapter/Easing.h>
#include <Lib/Adapter/Random.h>

//============================================================================*/
//	Enemy classMethods
//============================================================================*/

void Enemy::Init(uint32_t index, const Vector3& translate, Player* player) {

	player_ = player;

	BaseGameObject::Init("slime.obj");

	BaseGameObject::SetMeshRenderer("enemy", index);

	// 初期座標設定
	transform_.translation = translate;
	transform_.translation.y = initPosY_;
	transform_.scale.y = initScaleY_;
	transform_.Update();

	// Collider設定
	Collider::SetCollisionShapeSphere(); //* 球の形で設定
	Collider::radius_ = 1.2f;

	// 自身のタイプと攻撃対象
	Collider::type_ = ColliderType::Type_Enemy;
	Collider::targetType_ = ColliderType::Type_PlayerAttack;
	// Debug
	Collider::name_ = "enemy" + std::to_string(index);

	// 仮の色
	materials_.front().properties.color = Color(0.37f, 0.5f, 0.73f, 1.0f);

	velocity_.Init();
	moveVelocity_.Init();

	isHitDamage_ = false;
	isSetVelocity_ = false;
	isAlive_ = true;

	currentHp_ = maxHp_;

	aliveTimer_ = 0.0f;
	aliveTime_ = 1.2f;

	moveCoolTimer_ = 0.0f;
	moveCoolTime_ = Random::Generate(1.8f, 3.0f);

	startAnimationTimer_ = 0.0f;
	startAnimationTime_ = 1.0f;

	isAnimationFinish_ = false;

}

void Enemy::Update() {

	// 初期Animation
	StartAnimation();
	// ダメージを受けた時のAnimation
	KnockbackAnimation();
	// hpがなくなった時のAnimation
	DeadAnimation();

	Move();

	Collider::centerPos_ = transform_.translation;
	Collider::SphereUpdate();

	// 回転の更新
	RotateToDirection();

	BaseGameObject::Update();

}

void Enemy::Draw(RendererPipelineType pipeline) {

	BaseGameObject::Draw(pipeline);

}

void Enemy::OnCollisionEnter(Collider* other) {

	if (currentHp_ == 0) {
		return;
	}

	if (other->GetType() == this->targetType_) {

		currentHp_--;
		isHitDamage_ = true;
		isSetVelocity_ = true;
		if (currentHp_ == 0) {
			currentHp_ = 0;
			isHitDamage_ = false;
		}

		// 向き取得
		hitDirection_ = transform_.translation - player_->GetWorldTransform().translation;
		hitDirection_.Normalize();

		// DeltaTimeScaleを0.0fにする
		GameTimer::SetTimeScale(0.0f);
	}
}

void Enemy::StartAnimation() {

	if (isAnimationFinish_) {
		return;
	}

	startAnimationTimer_ += GameTimer::GetDeltaTime();
	float t = startAnimationTimer_ / startAnimationTime_;
	float easedT = EaseOutBounce(t);

	transform_.translation.y = std::lerp(initPosY_, 0.0f, easedT);
	transform_.scale.y = std::lerp(initScaleY_, 1.0f, easedT);

	if (t > 1.0f) {

		isAnimationFinish_ = true;
	}

}

void Enemy::KnockbackAnimation() {

	if (!isHitDamage_) {
		return;
	}

	// 初速度を設定
	if (isSetVelocity_) {
		const float knockbackSpeed = 0.4f; //* ノックバックの速度
		const float upwardVelocity = 4.0f; //* 上方向の速度
		velocity_ += hitDirection_ * knockbackSpeed;
		velocity_.y += upwardVelocity;

		moveVelocity_.Init();
		moveCoolTimer_ = 0.0f;

		isSetVelocity_ = false;
	}

	const float dampingFactor = 0.98f; // 減衰係数
	velocity_ *= dampingFactor;

	velocity_.y += gravity * GameTimer::GetScaledDeltaTime();
	transform_.translation += velocity_ * GameTimer::GetScaledDeltaTime();

	if (transform_.translation.y < groundY) {

		// 地面に衝突したら速度を反射
		transform_.translation.y = groundY;
		velocity_ = Vector3::Reflect(velocity_, Direction::Up());

		// 反射後に減衰を追加
		const float bounceDamping = 0.8f;
		velocity_.y *= bounceDamping;

		if (std::abs(velocity_.y) < 0.01f) {
			velocity_.Init();
			isHitDamage_ = false;
		}
	}

}

void Enemy::DeadAnimation() {

	if (currentHp_ != 0) {
		return;
	}

	// 初速度を設定
	if (isSetVelocity_) {
		const float knockbackSpeed = 3.0f; //* ノックバックの速度
		const float upwardVelocity = 10.0f; //* 上方向の速度
		velocity_ = hitDirection_ * knockbackSpeed;
		velocity_.y += upwardVelocity;

		moveVelocity_.Init();
		moveCoolTimer_ = 0.0f;

		isSetVelocity_ = false;
	}

	const float dampingFactor = 0.98f; // 減衰係数
	velocity_ *= dampingFactor;

	velocity_.y += gravity * GameTimer::GetDeltaTime();
	transform_.translation += velocity_ * GameTimer::GetDeltaTime();

	aliveTimer_ += GameTimer::GetDeltaTime();
	float t = aliveTimer_ / aliveTime_;
	float easedT = EaseOutExpo(t);
	rotateValue_ = std::lerp(0.0f, 0.12f, easedT);

	transform_.rotation =
		Quaternion::Multiply(transform_.rotation,
			Quaternion::MakeRotateAxisAngleQuaternion(Direction::Right(), -rotateValue_));
	if (t > 1.0f) {

		isAlive_ = false;
	}

}

void Enemy::Move() {

	if (!isAnimationFinish_ || currentHp_ == 0 || isHitDamage_) {
		return;
	}

	moveCoolTimer_ += GameTimer::GetDeltaTime();
	// 移動クールタイムが終わったら初速度を与える
	if (moveCoolTimer_ > moveCoolTime_) {

		const float moveSpeed = 0.25f; //* 移動速度
		const float upwardVelocity = 6.0f; //* 上方向の速度

		Vector3 direction = player_->GetWorldTransform().translation - transform_.translation;
		direction.Normalize();

		moveVelocity_ = direction * moveSpeed;
		moveVelocity_.y += upwardVelocity;

		// クールタイムをリセット
		moveCoolTimer_ = 0.0f;
	}

	const float dampingFactor = 0.98f; // 減衰係数
	moveVelocity_ *= dampingFactor;

	moveVelocity_.y += gravity * GameTimer::GetDeltaTime();
	transform_.translation += moveVelocity_ * GameTimer::GetDeltaTime();

	if (transform_.translation.y < groundY) {

		transform_.translation.y = 0.0f;
		moveVelocity_.Init();
	}
}

void Enemy::RotateToDirection() {

	if (currentHp_ == 0) {
		return;
	}

	// 方向ベクトル
	Vector3 forward =
		(player_->GetWorldTransform().translation - transform_.translation).Normalize();
	forward.y = 0.0f;
	// Playerの方を向かせる
	transform_.rotation = Quaternion::LookRotation(forward, Direction::Up());
}