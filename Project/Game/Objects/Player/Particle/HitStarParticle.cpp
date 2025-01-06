#include "HitStarParticle.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/Random.h>

//============================================================================*/
//	HitStarParticle classMethods
//============================================================================*/

void HitStarParticle::Init() {

	BaseParticle::SetParticleRenderer("hitStar");
	BaseParticle::Create("plane.obj");

	particleSystem_->SetTexture(GetName(), "starCircle");

	//* 固定設定 *//

	blendMode_ = kBlendModeAdd;             // 加算
	parameter_.isUniform = true;            // 均一に出る
	parameter_.isUseBillboard = true;       // Billboard有効
	parameter_.isUseScaledDeltaTime = true; // ScaleTime適応
	parameter_.moveToDirection = false;     // 回転を進行方向に合わせない

	parameter_.color = Color(0.802f, 0.321f, 0.321f, 1.0f);

	parameter_.easingType = EasingType::EaseInBounce;

}

void HitStarParticle::Update() {

	particleSystem_->Update();
}

void HitStarParticle::EmitOnce(const Vector3& translate) {

	// 座標設定
	parameter_.aabb.center = translate;

	// 短めに設定
	parameter_.lifeTime = Random::Generate(0.5f, 0.7f);
	parameter_.speed = Random::Generate(0.0f, 0.0f);

	// 範囲
	parameter_.aabb.extent = Vector3(
		Random::Generate(0.0f, 2.0f),
		Random::Generate(0.0f, 2.0f),
		Random::Generate(0.0f, 2.0f));

	// サイズ
	parameter_.scale.uniform = Vector3(
		Random::Generate(0.6f, 0.8f),
		Random::Generate(0.6f, 0.8f),
		Random::Generate(0.6f, 0.8f));

	parameter_.count = Random::Generate(3, 5);

	// 1度だけEmit
	BaseParticle::Emit();

}