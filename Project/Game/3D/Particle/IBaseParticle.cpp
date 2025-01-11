#include "IBaseParticle.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/ParticleRenderer.h>

//============================================================================*/
//	IBaseParticle classMethods
//============================================================================*/

IBaseParticle::~IBaseParticle() {
	ParticleRenderer::EraseParticle(this);
}

void IBaseParticle::SetParticleRenderer(const std::string& name, uint32_t index) {

	name_ = name;
	if (index != 0) {

		name_ = name_ + std::to_string(index);
	}

	ParticleRenderer::SetParticle(this);
}