#include "HitLineParticle.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/Random.h>

//============================================================================*/
//	HitLineParticle classMethods
//============================================================================*/

HitLineParticle::HitLineParticle(HitLineParticleType type) {
	type_ = type;
}

void HitLineParticle::Init() {

	std::string particleName;
	if (type_ == HitLineParticleType::RedWhite) {
		particleName = "hitLine_RedWhite";
	} else if (type_ == HitLineParticleType::Red) {
		particleName = "hitLine_Red";
	}

	BaseParticle::SetParticleRenderer(particleName);
	BaseParticle::Create("hitLine.obj");

	//* 固定設定 *//

	blendMode_ = kBlendModeAdd;             // 加算
	parameter_.isUniform = true;            // 均一に出る
	parameter_.isUseBillboard = false;      // Billboard無効
	parameter_.isUseScaledDeltaTime = true; // ScaleTime適応
	parameter_.moveToDirection = true;      // 回転を進行方向に合わせる

	//* 分岐固定初期化処理 *//

	if (type_ == HitLineParticleType::RedWhite) {

		parameter_.color = Color(0.987f, 0.554f, 0.554f, 1.0f);

		parameter_.count = 12;

		parameter_.easingType = EasingType::EaseInCubic;

	} else if (type_ == HitLineParticleType::Red) {

		parameter_.color = std::nullopt;
		parameter_.randomParticleColor = RandomParticleColor::RED;

		parameter_.count = 32;

		parameter_.easingType = EasingType::EaseInCubic;

	}

}

void HitLineParticle::Update() {

	particleSystem_->Update();
}

void HitLineParticle::EmitOnce(const Vector3& translate) {

	if (type_ == HitLineParticleType::RedWhite) {

		// 短めに設定
		parameter_.lifeTime = Random::Generate(0.5f, 0.7f);
		parameter_.speed = Random::Generate(6.0f, 8.0f);

		// サイズ Zのみランダム
		parameter_.scale.uniform = Vector3(
			1.0f,1.0f,Random::Generate(0.25f, 0.8f));

		// 範囲
		parameter_.aabb.extent = Vector3(
			Random::Generate(0.0f, 0.75f),
			Random::Generate(0.0f, 0.75f),
			Random::Generate(0.0f, 0.75f));

	} else if (type_ == HitLineParticleType::Red) {

		// 長めに設定
		parameter_.lifeTime = Random::Generate(0.5f, 0.7f);
		parameter_.speed = Random::Generate(12.0f, 16.0f);

		// サイズ Zのみランダム
		parameter_.scale.uniform = Vector3(
			1.0f, 1.0f, Random::Generate(1.0f, 2.0f));

		// 範囲
		parameter_.aabb.extent = Vector3(
			Random::Generate(0.0f, 0.75f),
			Random::Generate(0.0f, 0.75f),
			Random::Generate(0.0f, 0.75f));
	}

	// 座標設定
	parameter_.aabb.center = translate;
	parameter_.aabb.center.y += 1.0f;

	// 1度だけEmit
	BaseParticle::Emit();

}