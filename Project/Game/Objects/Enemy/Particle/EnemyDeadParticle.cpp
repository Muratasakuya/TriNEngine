#include "EnemyDeadParticle.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/Random.h>

//============================================================================*/
//	EnemyDeadParticle class
//============================================================================*/

EnemyDeadParticle::EnemyDeadParticle(uint32_t index) {
	index_ = index;
}

void EnemyDeadParticle::Init() {

	BaseParticle::SetParticleRenderer("circle", index_);
	BaseParticle::Create("plane.obj");

	particleSystem_->SetTexture(GetName(), "circle");

	//* 固定設定 *//

	blendMode_ = kBlendModeAdd;             // 加算
	parameter_.isUniform = true;            // 均一に出る
	parameter_.isUseBillboard = true;       // Billboard有効
	parameter_.isUseScaledDeltaTime = true; // ScaleTime適応
	parameter_.moveToDirection = false;     // 回転を進行方向に合わせない

	parameter_.randomParticleColor = RandomParticleColor::BLUE;

	parameter_.easingType = EasingType::EaseInExpo;

}

void EnemyDeadParticle::Update() {

	particleSystem_->Update();
}

void EnemyDeadParticle::EmitOnce(const Vector3& translate) {

	// 座標設定
	parameter_.aabb.center = translate;

	// 短めに設定
	parameter_.lifeTime = Random::Generate(1.0f, 1.4f);
	parameter_.speed = Random::Generate(8.0f, 12.0f);

	// 範囲
	parameter_.aabb.extent = Vector3(
		Random::Generate(0.0f, 0.5f),
		Random::Generate(0.0f, 0.5f),
		Random::Generate(0.0f, 0.5f));

	// サイズ
	parameter_.scale.uniform = Vector3(
		Random::Generate(0.2f, 0.8f),
		Random::Generate(0.2f, 0.8f),
		Random::Generate(0.2f, 0.8f));

	parameter_.count = Random::Generate(64, 88);

	// 1度だけEmit
	BaseParticle::Emit();

}