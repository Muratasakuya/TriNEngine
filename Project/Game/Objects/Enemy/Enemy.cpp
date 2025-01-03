#include "Enemy.h"

//============================================================================*/
//	Enemy classMethods
//============================================================================*/

void Enemy::Init(uint32_t index) {

	BaseGameObject::Init("slime.obj");

	BaseGameObject::SetMeshRenderer("enemy", index);

	// Collider設定
	Collider::SetCollisionShapeSphere(); //* 球の形で設定
	Collider::radius_ = 1.2f;

	// 自身のタイプと攻撃対象
	Collider::type_ = ColliderType::Type_Enemy;
	Collider::targetType_ = ColliderType::Type_PlayerAttack;
	// Debug
	Collider::name_ = "enemy" + std::to_string(index);

	isAlive_ = true;

}

void Enemy::Update() {

	Collider::centerPos_ = transform_.translation;
	Collider::SphereUpdate();

	BaseGameObject::Update();

}

void Enemy::Draw(RendererPipelineType pipeline) {

	BaseGameObject::Draw(pipeline);

	Collider::DrawCollider();

}

void Enemy::OnCollisionEnter(Collider* other) {

	if (other->GetType() == this->targetType_) {

		isAlive_ = false;
	}
}