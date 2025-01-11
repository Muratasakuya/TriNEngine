#include "ParticleParameter.h"

//============================================================================*/
//	include
//============================================================================*/

// front
#include <Game/3D/Particle/ParticleVisitor.h>

//============================================================================*/
//	ParticleParameter classMethods
//============================================================================*/

void ParticleParameter::Accept(ParticleVisitor& visitor, std::list<ParticleData>& particles) {

	scale.Init();

	visitor.Visit(particles, *this);
}