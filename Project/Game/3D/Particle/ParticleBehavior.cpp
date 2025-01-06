#include "ParticleBehavior.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>
#include <Game/3D/Particle/ParticleVisitor.h>
#include <Lib/Adapter/Random.h>

//============================================================================*/
//	DispersionBehavior classMethods
//============================================================================*/

void DispersionBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	DispersionVisitor visitor;
	parameter.Accept(visitor, particles);
}

void DispersionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += GameTimer::GetDeltaTime();

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* GameTimer::GetDeltaTime(),
			easedVelocity.y* GameTimer::GetDeltaTime(),
			easedVelocity.z* GameTimer::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

}

//============================================================================*/
//	ChaseBehavior classMethods
//============================================================================*/

void ChaseBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ChaseVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ChaseBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += GameTimer::GetDeltaTime();

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* GameTimer::GetDeltaTime(),
			easedVelocity.y* GameTimer::GetDeltaTime(),
			easedVelocity.z* GameTimer::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

}

//============================================================================*/
//	ConvergeBehavior classMethods
//============================================================================*/

void ConvergeBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ConvergeVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ConvergeBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += GameTimer::GetDeltaTime();

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);

	Vector3 easedVelocity = particle.velocity * easedLifeRatio;
	particle.transform.translate += {
		easedVelocity.x* GameTimer::GetDeltaTime(),
			easedVelocity.y* GameTimer::GetDeltaTime(),
			easedVelocity.z* GameTimer::GetDeltaTime()
	};

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

}

//============================================================================*/
//	InjectionBehavior classMethods
//============================================================================*/

void InjectionBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	InjectionVisitor visitor;
	parameter.Accept(visitor, particles);
}

void InjectionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix) {

	particle.currentTime += GameTimer::GetDeltaTime();

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);

	// イージング処理
	if (particle.easingType.has_value()) {
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}
	float easedLifeRatio = particle.easedLifeRatio.value_or(lifeRatio);
	Vector3 easedVelocity = particle.velocity * easedLifeRatio;

	if (particle.physics.reflectEnable) {

		// 移動処理前の位置
		Vector3 newPosition = particle.transform.translate + easedVelocity * GameTimer::GetDeltaTime();

		// 衝突判定
		if (newPosition.y <= particle.physics.reflectFace.y && particle.velocity.y < 0) {

			particle.velocity = Vector3::Reflect(particle.velocity, Direction::Up()) * particle.physics.restitution;

			// 位置の調整
			newPosition.y = particle.physics.reflectFace.y;
		}

		// 更新された位置に適用
		particle.transform.translate = newPosition;
	} else {

		particle.transform.translate += {
			easedVelocity.x* GameTimer::GetDeltaTime(),
				easedVelocity.y* GameTimer::GetDeltaTime(),
				easedVelocity.z* GameTimer::GetDeltaTime()
		};
	}

	Vector3 gravityEffect =
		particle.physics.gravityDirection.value() * particle.physics.gravityStrength.value() * GameTimer::GetDeltaTime();
	particle.velocity += gravityEffect;

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	particle.wvpMatrix = particle.worldMatrix * GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();

}

//============================================================================*/
//	ParticleBehaviorFactory classMethods
//============================================================================*/

std::unique_ptr<ParticleBehavior> ParticleBehaviorFactory::CreateBehavior(ParticleType particleType) {

	switch (particleType) {
	case ParticleType::kDispersion:

		return std::make_unique<DispersionBehavior>();
	case ParticleType::kChase:

		return std::make_unique<ChaseBehavior>();
	case ParticleType::kConverge:

		return std::make_unique<ConvergeBehavior>();
	case ParticleType::kInjection:

		return std::make_unique<InjectionBehavior>();
	}

	throw std::runtime_error("unKnown particleType");
}