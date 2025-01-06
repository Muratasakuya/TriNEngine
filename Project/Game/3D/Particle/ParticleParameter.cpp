#include "ParticleParameter.h"

//============================================================================*/
//	include
//============================================================================*/

// front
#include <Game/3D/Particle/ParticleVisitor.h>

//============================================================================*/
//	ParticleParameter classMethods
//============================================================================*/

void DispersionParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}
void ChaseParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}
void ConvergeParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}
void InjectionParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {
	visitor.Visit(particles, *this);
}