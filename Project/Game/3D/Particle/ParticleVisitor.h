#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/ParticleStructure.h>

// c++
#include <vector>
#include <list>
#include <optional>

// front
class ParticleParameter;

//============================================================================*/
//	ParticleVisitor class
//============================================================================*/
class ParticleVisitor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleVisitor() = default;
	virtual ~ParticleVisitor() = default;

	virtual void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;
	virtual void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;
	virtual void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) = 0;

	Color SettingColor(const std::optional<Color>& color, const std::optional<RandomParticleColor>& randomParticleColor);

};

//============================================================================*/
//	DispersionVisitor class
//============================================================================*/
class DispersionVisitor
	: public ParticleVisitor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DispersionVisitor() = default;
	~DispersionVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//============================================================================*/
//	ChaseVisitor class
//============================================================================*/
class ChaseVisitor
	: public ParticleVisitor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ChaseVisitor() = default;
	~ChaseVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//============================================================================*/
//	ConvergeVisitor class
//============================================================================*/
class ConvergeVisitor
	: public ParticleVisitor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ConvergeVisitor() = default;
	~ConvergeVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};

//============================================================================*/
//	ConvergeVisitor class
//============================================================================*/
class InjectionVisitor
	: public ParticleVisitor {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InjectionVisitor() = default;
	~InjectionVisitor() = default;

	void Visit(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;
	void CreateNonUniformParticles(std::list<ParticleData>& particles, ParticleParameter& parameter) override;

};