#include "ParticleVisitor.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/Adapter/Random.h>

// front
#include <Game/3D/Particle/ParticleParameter.h>

//============================================================================*/
//	ParticleVisitor classMethods
//============================================================================*/

Color ParticleVisitor::SettingColor(
	const std::optional<Color>& color, const std::optional<RandomParticleColor>& randomParticleColor) {

	Color outputColor{};

	if (color.has_value()) {

		outputColor = color.value();
	} else {
		if (randomParticleColor.has_value()) {

			RandomParticleColor randomColor = randomParticleColor.value();
			switch (randomColor) {
			case RandomParticleColor::RED:
				outputColor = Color(
					Random::Generate(0.75f, 1.0f), // 赤成分を強く
					Random::Generate(0.0f, 0.2f), // 緑成分を弱め
					Random::Generate(0.0f, 0.2f), // 青成分を弱め
					1.0f);
				break;
			case RandomParticleColor::GREEN:
				outputColor = Color(
					Random::Generate(0.0f, 0.2f), // 赤成分を弱め
					Random::Generate(0.75f, 1.0f), // 緑成分を強く
					Random::Generate(0.0f, 0.2f), // 青成分を弱め
					1.0f);
				break;
			case RandomParticleColor::BLUE:
				outputColor = Color(
					Random::Generate(0.0f, 0.2f), // 赤成分を弱め
					Random::Generate(0.0f, 0.2f), // 緑成分を弱め
					Random::Generate(0.75f, 1.0f), // 青成分を強く
					1.0f);
				break;
			case RandomParticleColor::PURPLE:
				outputColor = Color(
					Random::Generate(0.75f, 1.0f), // 赤成分を強く
					Random::Generate(0.0f, 0.2f), // 緑成分を弱め
					Random::Generate(0.75f, 1.0f), // 青成分を強く
					1.0f);
				break;
			case RandomParticleColor::GRAY:
			{
				float grayValue = Random::Generate(0.3f, 0.7f); // 灰色は赤緑青が均等
				outputColor = Color(
					grayValue,
					grayValue,
					grayValue,
					1.0f);
				break;
			}
			case RandomParticleColor::DARKBLUE:
				outputColor = Color(
					Random::Generate(0.0f, 0.1f),  // 赤成分をほぼ暗く
					Random::Generate(0.0f, 0.1f),  // 緑成分もほぼ暗く
					Random::Generate(0.4f, 0.8f),  // 青成分を中～高い範囲に設定
					1.0f);
				break;
			}
		} else {

			outputColor = Color(
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				Random::Generate(0.0f, 1.0f),
				1.0f);
		}
	}

	return outputColor;
}

//============================================================================*/
//	DispersionVisitor classMethods
//============================================================================*/

void DispersionVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void DispersionVisitor::CreateUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& dispersionParameter = static_cast<DispersionParticleParameter&>(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < dispersionParameter.count; ++index) {

		ParticleData particle{};
		particle.currentTime = 0.0f;

		float theta = 2.0f * std::numbers::pi_v<float> *(index / goldenRatio);
		float phi = std::acos(1.0f - 2.0f * (index + 0.5f) / dispersionParameter.count);

		float x = std::sin(phi) * std::cos(theta);
		float y = std::sin(phi) * std::sin(theta);
		float z = std::cos(phi);
		Vector3 particlePos = Vector3(x, y, z);

		const Vector3& min = dispersionParameter.aabb.GetMin();
		const Vector3& max = dispersionParameter.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		particle.transform.translate = particlePos + randomPos;

		float speedFactor = 1.0f + 0.5f * (1.0f - z);
		particle.velocity = Vector3(x, y, z) * speedFactor * dispersionParameter.speed.value_or(1.0f);

		particle.transform.rotate.SetInit(0.0f);
		particle.transform.scale = dispersionParameter.scale.uniform;

		particle.lifeTime = dispersionParameter.lifeTime.value_or(Random::Generate(2.0f, 3.0f));

		particle.color = SettingColor(dispersionParameter.color, dispersionParameter.randomParticleColor);

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

void DispersionVisitor::CreateNonUniformParticles(
	std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& dispersionParameter = static_cast<DispersionParticleParameter&>(parameter);

	for (size_t index = 0; index < dispersionParameter.count; ++index) {

		ParticleData particle{};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		const Vector3& min = dispersionParameter.aabb.GetMin();
		const Vector3& max = dispersionParameter.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		particle.transform.translate = randomPos;

		particle.transform.scale = Vector3(
			Random::Generate(dispersionParameter.scale.min.x, dispersionParameter.scale.max.x),
			Random::Generate(dispersionParameter.scale.min.y, dispersionParameter.scale.max.y),
			Random::Generate(dispersionParameter.scale.min.z, dispersionParameter.scale.max.z));

		if (dispersionParameter.speed.has_value()) {
			particle.velocity = Vector3(
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()),
				Random::Generate(-dispersionParameter.speed.value(), dispersionParameter.speed.value()));
		} else {
			particle.velocity = Vector3(
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f),
				Random::Generate(-1.0f, 1.0f));
		}

		particle.color = SettingColor(dispersionParameter.color, dispersionParameter.randomParticleColor);

		particle.lifeTime = dispersionParameter.lifeTime.value_or(Random::Generate(1.0f, 3.0f));

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

//============================================================================*/
//	ChaseVisitor classMethods
//============================================================================*/

void ChaseVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void ChaseVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const ChaseParticleParameter& chaseParticle = static_cast<ChaseParticleParameter&>(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index) {

		ParticleData particle{};

		particle.currentTime = 0.0f;

		const Vector3& min = chaseParticle.aabb.GetMin();
		const Vector3& max = chaseParticle.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		particle.transform.translate = randomPos;

		particle.transform.scale = chaseParticle.scale.uniform;

		Vector3 direction = Vector3(chaseParticle.aabb.center - chaseParticle.prePos).Normalize();
		particle.velocity = chaseParticle.speed.value_or(0.5f) * direction;

		float angle = std::atan2(direction.y, direction.x);
		particle.transform.rotate.SetInit(angle);

		particle.color = SettingColor(chaseParticle.color, chaseParticle.randomParticleColor);

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.aabb.center == chaseParticle.prePos) {

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

void ChaseVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const ChaseParticleParameter& chaseParticle = static_cast<ChaseParticleParameter&>(parameter);

	for (size_t index = 0; index < chaseParticle.count; ++index) {

		ParticleData particle{};

		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		const Vector3& min = chaseParticle.aabb.GetMin();
		const Vector3& max = chaseParticle.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		particle.transform.translate = randomPos;

		particle.transform.scale = Vector3(
			Random::Generate(chaseParticle.scale.min.x, chaseParticle.scale.max.x),
			Random::Generate(chaseParticle.scale.min.y, chaseParticle.scale.max.y),
			Random::Generate(chaseParticle.scale.min.z, chaseParticle.scale.max.z));

		Vector3 direction = Vector3(chaseParticle.aabb.center - chaseParticle.prePos).Normalize();

		Vector3 randomOffset{};
		randomOffset.SetInit(Random::Generate(-0.4f, 0.4f));
		randomOffset.z = 0.0f;
		Vector3 spreadDirection = Vector3(direction + randomOffset).Normalize();

		if (chaseParticle.speed.has_value()) {
			particle.velocity = chaseParticle.speed.value() * spreadDirection;
		} else {
			particle.velocity = Random::Generate(0.4f, 0.8f) * spreadDirection;
		}

		particle.color = SettingColor(chaseParticle.color, chaseParticle.randomParticleColor);

		particle.lifeTime = chaseParticle.lifeTime.value_or(Random::Generate(1.0f, 2.0f));

		if (chaseParticle.aabb.center == chaseParticle.prePos) {

			particle.lifeTime = 0.0f;
		}

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

//============================================================================*/
//	ConvergeVisitor classMethods
//============================================================================*/

void ConvergeVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void ConvergeVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& convergeParameter = static_cast<ConvergeParticleParameter&>(parameter);

	//* FibonacciSphereMethod *//

	const float goldenRatio = (1.0f + std::sqrtf(5.0f)) / 2.0f;

	for (size_t index = 0; index < convergeParameter.count; ++index) {

		ParticleData particle{};
		particle.transform.rotate.SetInit(0.0f);
		particle.currentTime = 0.0f;

		float theta = 2.0f * std::numbers::pi_v<float> *(index / goldenRatio);
		float phi = std::acos(1.0f - 2.0f * (index + 0.5f) / convergeParameter.count);

		float x = std::sin(phi) * std::cos(theta);
		float y = std::sin(phi) * std::sin(theta);
		float z = std::cos(phi);

		const Vector3& min = convergeParameter.aabb.GetMin();
		const Vector3& max = convergeParameter.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		Vector3 initialPos = Vector3(x, y, z) + randomPos;
		particle.transform.translate = initialPos;

		Vector3 targetDirection = Vector3(convergeParameter.aabb.center - initialPos).Normalize();
		float speed = convergeParameter.speed.value_or(0.5f);
		particle.velocity = targetDirection * speed;

		particle.transform.scale = convergeParameter.scale.uniform;

		float distanceToTarget = Vector3(convergeParameter.aabb.center - initialPos).Length();
		float timeToReachTarget = distanceToTarget / speed;
		particle.lifeTime = convergeParameter.lifeTime.value_or(timeToReachTarget);

		particle.color = SettingColor(convergeParameter.color, convergeParameter.randomParticleColor);

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

void ConvergeVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& convergeParameter = static_cast<ConvergeParticleParameter&>(parameter);

	for (size_t index = 0; index < convergeParameter.count; ++index) {

		ParticleData particle{};
		particle.currentTime = 0.0f;
		particle.transform.rotate.SetInit(0.0f);

		const Vector3& min = convergeParameter.aabb.GetMin();
		const Vector3& max = convergeParameter.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));

		particle.transform.translate = randomPos;

		Vector3 targetDirection = Vector3(convergeParameter.aabb.center - particle.transform.translate).Normalize();
		float speed = convergeParameter.speed.value_or(Random::Generate(0.3f, 0.7f));
		particle.velocity = targetDirection * speed;

		particle.transform.scale = Vector3(
			Random::Generate(convergeParameter.scale.min.x, convergeParameter.scale.max.x),
			Random::Generate(convergeParameter.scale.min.y, convergeParameter.scale.max.y),
			Random::Generate(convergeParameter.scale.min.z, convergeParameter.scale.max.z));

		particle.color = convergeParameter.color.value_or(Color(
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			Random::Generate(0.0f, 1.0f),
			1.0f));

		float distanceToTarget = Vector3(convergeParameter.aabb.center - particle.transform.translate).Length();
		float timeToReachTarget = distanceToTarget / speed;
		particle.lifeTime = convergeParameter.lifeTime.value_or(timeToReachTarget);

		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}
}

//============================================================================*/
//	InjectionVisitor classMethods
//============================================================================*/

void InjectionVisitor::Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	if (parameter.isUniform) {

		CreateUniformParticles(particles, parameter);
	} else {

		CreateNonUniformParticles(particles, parameter);
	}
}

void InjectionVisitor::CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& injectionParameter = static_cast<InjectionParticleParameter&>(parameter);

	Vector3 baseDirection = injectionParameter.injectionDirection;
	baseDirection.Normalize();

	for (size_t index = 0; index < injectionParameter.count; ++index) {

		ParticleData particle{};

		particle.currentTime = 0.0f;

		particle.transform.translate = injectionParameter.aabb.center;

		particle.transform.scale = injectionParameter.scale.uniform;

		float angleStep = std::numbers::pi_v<float> *2.0f / injectionParameter.count;
		float currentAngle = angleStep * index;

		Vector3 spreadDirection(
			std::cos(currentAngle), // X成分
			0.0f,                   // Y成分は固定
			std::sin(currentAngle)  // Z成分
		);
		spreadDirection.Normalize();

		// velocityの設定
		particle.velocity = injectionParameter.speed.value_or(0.5f) * spreadDirection;
		particle.velocity.y = injectionParameter.speed.value_or(4.0f);

		particle.color = SettingColor(injectionParameter.color, injectionParameter.randomParticleColor);

		particle.physics.gravityDirection = injectionParameter.physics.gravityDirection.value_or(Vector3(0.0f, -1.0f, 0.0f));
		particle.physics.gravityStrength = injectionParameter.physics.gravityStrength.value_or(9.8f);
		particle.physics.reflectFace = injectionParameter.physics.reflectFace;
		particle.physics.restitution = injectionParameter.physics.restitution;
		particle.physics.reflectEnable = injectionParameter.physics.reflectEnable;

		particle.lifeTime = injectionParameter.lifeTime.value_or(6.0f);
		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}

}

void InjectionVisitor::CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) {

	const auto& injectionParameter = static_cast<InjectionParticleParameter&>(parameter);

	for (size_t index = 0; index < injectionParameter.count; ++index) {

		ParticleData particle{};

		particle.currentTime = 0.0f;

		const Vector3& min = injectionParameter.aabb.GetMin();
		const Vector3& max = injectionParameter.aabb.GetMax();
		Vector3 randomPos = Vector3(
			Random::Generate(min.x, max.x),
			Random::Generate(min.y, max.y),
			Random::Generate(min.z, max.z));
		particle.transform.translate = randomPos;

		particle.transform.scale = Vector3(
			Random::Generate(injectionParameter.scale.min.x, injectionParameter.scale.max.x),
			Random::Generate(injectionParameter.scale.min.y, injectionParameter.scale.max.y),
			Random::Generate(injectionParameter.scale.min.z, injectionParameter.scale.max.z));

		Vector3 randomOffset = Vector3(
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f),
			Random::Generate(-1.0f, 1.0f));
		float randomnessStrength = 0.5f;

		Vector3 finalDirection = injectionParameter.injectionDirection + (randomOffset * randomnessStrength);
		finalDirection.Normalize();

		particle.velocity = injectionParameter.speed.value_or(0.5f) * finalDirection;

		particle.color = SettingColor(injectionParameter.color, injectionParameter.randomParticleColor);

		particle.physics.gravityDirection = injectionParameter.physics.gravityDirection.value_or(Vector3(0.0f, -1.0f, 0.0f));
		particle.physics.gravityStrength = injectionParameter.physics.gravityStrength.value_or(9.8f);
		particle.physics.reflectFace = injectionParameter.physics.reflectFace;
		particle.physics.restitution = injectionParameter.physics.restitution;
		particle.physics.reflectEnable = injectionParameter.physics.reflectEnable;

		particle.lifeTime = injectionParameter.lifeTime.value_or(Random::Generate(2.0f, 8.0f));
		particle.easingType = parameter.easingType;

		particle.isUseScaledDeltaTime = parameter.isUseScaledDeltaTime;
		particle.isUseBillboard = parameter.isUseBillboard;
		particle.moveToDirection = parameter.moveToDirection;

		particles.push_back(particle);
	}

}