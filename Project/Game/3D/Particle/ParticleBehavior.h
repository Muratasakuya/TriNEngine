#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/ParticleParameter.h>
#include <Game/Utility/Direction.h>

// c++
#include <memory>
#include <vector>
#include <list>

//** Types **//
enum ParticleType {

	kDispersion, // 分散
	kChase,      // 追跡
	kConverge,   // 収束
	kInjection,  // 噴射
};

namespace ParticleBehaviorUpdate {


}

//============================================================================*/
//	ParticleBehavior class
//============================================================================*/
class ParticleBehavior {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleBehavior() = default;
	virtual ~ParticleBehavior() = default;

	virtual void Create(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;

	virtual void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) = 0;

	//* getter *//

	virtual ParticleType GetType() const { return type_; }

protected:
	//========================================================================*/
	//	protected Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ParticleType type_;

};

//============================================================================*/
//	DispersionBehavior class
//============================================================================*/
class DispersionBehavior
	: public ParticleBehavior {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DispersionBehavior() { type_ = ParticleType::kDispersion; };
	~DispersionBehavior() = default;

	void Create(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

//============================================================================*/
//	ChaseBehavior class
//============================================================================*/
class ChaseBehavior
	: public ParticleBehavior {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ChaseBehavior() { type_ = ParticleType::kChase; };
	~ChaseBehavior() = default;

	void Create(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

//============================================================================*/
//	ConvergeBehavior class
//============================================================================*/
class ConvergeBehavior
	: public ParticleBehavior {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ConvergeBehavior() { type_ = ParticleType::kConverge; };
	~ConvergeBehavior() = default;

	void Create(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

//============================================================================*/
//	InjectionBehavior class
//============================================================================*/
class InjectionBehavior
	: public ParticleBehavior {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InjectionBehavior() { type_ = ParticleType::kInjection; };
	~InjectionBehavior() = default;

	void Create(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

	void Update(ParticleData& particle, const Matrix4x4& billboardMatrix) override;

};

//============================================================================*/
//	ParticleBehaviorFactory class
//============================================================================*/
class ParticleBehaviorFactory {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleBehaviorFactory() = default;
	~ParticleBehaviorFactory() = default;

	static std::unique_ptr<ParticleBehavior> CreateBehavior(ParticleType behaviorType);

};