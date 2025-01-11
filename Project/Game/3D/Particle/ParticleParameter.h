#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/ParticleStructure.h>
#include <Game/3D/Collision/CollisionShape.h>
#include <Engine/CBuffer/MaterialBuffer.h>

// c++
#include <list>
#include <vector>
#include <optional>

//* front
class ParticleVisitor;

//============================================================================*/
//	ParticleParameter class
//============================================================================*/
class ParticleParameter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleParameter() = default;
	virtual ~ParticleParameter() = default;

	void Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles);

	//* commonParameters *//

	bool isUniform = true; //* 均一か非均一か決めるフラグ

	ParticleValue scale; //* S
	AABB aabb;           //* Emiterの場所、範囲

	//* life
	std::optional<float> speed = std::nullopt;
	std::optional<float> lifeTime = std::nullopt;

	//* 色
	std::optional<Color> color = std::nullopt;
	std::optional<RandomParticleColor> randomParticleColor;

	// 物理
	ParticlePhysics physics;

	// scaleされたDeltaTimeを使うかどうか
	// Defaultはfalse
	bool isUseScaledDeltaTime = false;
	bool isUseBillboard = true;
	bool moveToDirection = false;

	//* easing
	std::optional<EasingType> easingType = std::nullopt;

	//* emitter 
	uint32_t count = 16;    // 個数
	float frequency = 4.0f; // ~秒置き、発生頻度
	float frequencyTime;    // 発生頻度用の時刻

};

//============================================================================*/
//	DispersionParticleParameter class
//============================================================================*/
class DispersionParticleParameter
	: public ParticleParameter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DispersionParticleParameter() = default;
	~DispersionParticleParameter() = default;
};

//============================================================================*/
//	ChaseParticleParameter class
//============================================================================*/
class ChaseParticleParameter
	: public ParticleParameter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ChaseParticleParameter() = default;
	~ChaseParticleParameter() = default;

	Vector3 prePos;

};

//============================================================================*/
//	ConvergeParticleParameter class
//============================================================================*/
class ConvergeParticleParameter
	: public ParticleParameter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ConvergeParticleParameter() = default;
	~ConvergeParticleParameter() = default;
};

//============================================================================*/
//	InjectionParticleParameter class
//============================================================================*/
class InjectionParticleParameter
	: public ParticleParameter {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InjectionParticleParameter() = default;
	~InjectionParticleParameter() = default;

	Vector3 injectionDirection;

};