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

void DispersionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix, ParticleRenderTarget renderTarget) {

	float deltaTime;
	if (particle.isUseScaledDeltaTime) {
		deltaTime = GameTimer::GetScaledDeltaTime();
	} else {
		deltaTime = GameTimer::GetDeltaTime();
	}
	// 現在の時刻を進める
	particle.currentTime += deltaTime;

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	// イージング処理
	if (particle.easingType.has_value()) {
		// EasedTにあたる
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	//============================================================================*/
	/// Translate

	// 速度をイージングにかける
	Vector3 easedVelocity = particle.velocity * particle.easedLifeRatio.value_or(lifeRatio);
	particle.transform.translate += easedVelocity * deltaTime;

	//============================================================================*/
	/// Scale

	// スケールをイージングにかける
	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Rotate: 回転を進行方向に合わせる

	Vector3 direction = easedVelocity.Normalize();
	Vector3 rotate{};

	if (particle.moveToDirection) {
		rotate.y = std::atan2(direction.x, direction.z);
		// 横軸方向の長さを求める
		float horizontalLength = std::sqrtf(direction.x * direction.x + direction.z * direction.z);
		// X軸周りの角度(θx)
		rotate.x = std::atan2(-direction.y, horizontalLength);
	} else {

		rotate.Init();
	}

	//============================================================================*/
	/// Color.a

	// α値をイージングにかける
	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Matrix

	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = Matrix4x4::MakeIdentity4x4();
	if (particle.isUseBillboard) {
		particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	} else {
		particle.worldMatrix = Matrix4x4::MakeAffineMatrix(scaledTransform, rotate, particle.transform.translate);
	}

	Matrix4x4 viewProjection = Matrix4x4::MakeIdentity4x4();
	if (renderTarget == ParticleRenderTarget::GameScene) {
		viewProjection = GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	} else if (renderTarget == ParticleRenderTarget::DemoScene) {
		viewProjection = GameSystem::GameCamera()->GetDemoDebugCamera()->GetViewProjectionMatrix();
	}
	particle.wvpMatrix = particle.worldMatrix * viewProjection;

}

//============================================================================*/
//	ChaseBehavior classMethods
//============================================================================*/

void ChaseBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ChaseVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ChaseBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix, ParticleRenderTarget renderTarget) {

	float deltaTime;
	if (particle.isUseScaledDeltaTime) {
		deltaTime = GameTimer::GetScaledDeltaTime();
	} else {
		deltaTime = GameTimer::GetDeltaTime();
	}
	// 現在の時刻を進める
	particle.currentTime += deltaTime;

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	// イージング処理
	if (particle.easingType.has_value()) {
		// EasedTにあたる
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	//============================================================================*/
	/// Translate

	// 速度をイージングにかける
	Vector3 easedVelocity = particle.velocity * particle.easedLifeRatio.value_or(lifeRatio);
	particle.transform.translate += easedVelocity * deltaTime;

	//============================================================================*/
	/// Scale

	// スケールをイージングにかける
	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Color.a

	// α値をイージングにかける
	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Matrix

	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = Matrix4x4::MakeIdentity4x4();
	if (particle.isUseBillboard) {
		particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	} else {
		particle.worldMatrix = scaleMatrix * translateMatrix;
	}

	Matrix4x4 viewProjection = Matrix4x4::MakeIdentity4x4();
	if (renderTarget == ParticleRenderTarget::GameScene) {
		viewProjection = GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	} else if (renderTarget == ParticleRenderTarget::DemoScene) {
		viewProjection = GameSystem::GameCamera()->GetDemoDebugCamera()->GetViewProjectionMatrix();
	}
	particle.wvpMatrix = particle.worldMatrix * viewProjection;
}

//============================================================================*/
//	ConvergeBehavior classMethods
//============================================================================*/

void ConvergeBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	ConvergeVisitor visitor;
	parameter.Accept(visitor, particles);
}

void ConvergeBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix, ParticleRenderTarget renderTarget) {

	float deltaTime;
	if (particle.isUseScaledDeltaTime) {
		deltaTime = GameTimer::GetScaledDeltaTime();
	} else {
		deltaTime = GameTimer::GetDeltaTime();
	}
	// 現在の時刻を進める
	particle.currentTime += deltaTime;

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	// イージング処理
	if (particle.easingType.has_value()) {
		// EasedTにあたる
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	//============================================================================*/
	/// Translate

	// 速度をイージングにかける
	Vector3 easedVelocity = particle.velocity * particle.easedLifeRatio.value_or(lifeRatio);
	particle.transform.translate += easedVelocity * deltaTime;


	//============================================================================*/
	/// Scale

	// スケールをイージングにかける
	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Color.a

	// α値をイージングにかける
	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Matrix

	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = Matrix4x4::MakeIdentity4x4();
	if (particle.isUseBillboard) {
		particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	} else {
		particle.worldMatrix = scaleMatrix * translateMatrix;
	}

	Matrix4x4 viewProjection = Matrix4x4::MakeIdentity4x4();
	if (renderTarget == ParticleRenderTarget::GameScene) {
		viewProjection = GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	} else if (renderTarget == ParticleRenderTarget::DemoScene) {
		viewProjection = GameSystem::GameCamera()->GetDemoDebugCamera()->GetViewProjectionMatrix();
	}
	particle.wvpMatrix = particle.worldMatrix * viewProjection;
}

//============================================================================*/
//	InjectionBehavior classMethods
//============================================================================*/

void InjectionBehavior::Create(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	InjectionVisitor visitor;
	parameter.Accept(visitor, particles);
}

void InjectionBehavior::Update(ParticleData& particle, const Matrix4x4& billboardMatrix, ParticleRenderTarget renderTarget) {

	float deltaTime;
	if (particle.isUseScaledDeltaTime) {
		deltaTime = GameTimer::GetScaledDeltaTime();
	} else {
		deltaTime = GameTimer::GetDeltaTime();
	}
	// 現在の時刻を進める
	particle.currentTime += deltaTime;

	// 寿命の進行度合い
	float lifeRatio = 1.0f - (particle.currentTime / particle.lifeTime);
	// イージング処理
	if (particle.easingType.has_value()) {
		// EasedTにあたる
		particle.easedLifeRatio = EasedValue(particle.easingType.value(), lifeRatio);
	}

	// 速度をイージングにかける
	Vector3 easedVelocity = particle.velocity * particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Physics

	// 反射するかどうか
	if (particle.physics.reflectEnable) {

		Vector3 newPosition = particle.transform.translate + easedVelocity * deltaTime;

		// 衝突判定
		if (newPosition.y <= particle.physics.reflectFace.y && particle.velocity.y < 0) {

			particle.velocity = Vector3::Reflect(particle.velocity, Direction::Up()) * particle.physics.restitution;

			// 位置の調整
			newPosition.y = particle.physics.reflectFace.y;
		}

		// 更新された位置に適用
		particle.transform.translate = newPosition;
	} else {

		particle.transform.translate += easedVelocity * deltaTime;
	}

	Vector3 gravityEffect =
		particle.physics.gravityDirection.value() * particle.physics.gravityStrength.value() * deltaTime;
	particle.velocity += gravityEffect;

	//============================================================================*/
	/// Color.a

	// α値をイージングにかける
	particle.color.a = particle.easedLifeRatio.value_or(lifeRatio);

	//============================================================================*/
	/// Matrix

	Vector3 scaledTransform = particle.transform.scale * particle.easedLifeRatio.value_or(lifeRatio);
	Matrix4x4 scaleMatrix =
		Matrix4x4::MakeScaleMatrix(scaledTransform);

	Matrix4x4 translateMatrix =
		Matrix4x4::MakeTranslateMatrix(particle.transform.translate);

	particle.worldMatrix = Matrix4x4::MakeIdentity4x4();
	if (particle.isUseBillboard) {
		particle.worldMatrix = scaleMatrix * billboardMatrix * translateMatrix;
	} else {
		particle.worldMatrix = scaleMatrix * translateMatrix;
	}

	Matrix4x4 viewProjection = Matrix4x4::MakeIdentity4x4();
	if (renderTarget == ParticleRenderTarget::GameScene) {
		viewProjection = GameSystem::GameCamera()->GetCamera3D()->GetViewProjectionMatrix();
	} else if (renderTarget == ParticleRenderTarget::DemoScene) {
		viewProjection = GameSystem::GameCamera()->GetDemoDebugCamera()->GetViewProjectionMatrix();
	}
	particle.wvpMatrix = particle.worldMatrix * viewProjection;
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