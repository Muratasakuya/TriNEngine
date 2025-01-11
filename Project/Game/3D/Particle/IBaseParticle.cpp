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

void IBaseParticle::SetParticleRenderer(const std::string& name, ParticleRenderTarget renderTarget) {

	name_ = name;

	if (renderTarget == ParticleRenderTarget::GameScene) {
		ParticleRenderer::SetParticle(this);
	} else if (renderTarget == ParticleRenderTarget::DemoScene) {
		ParticleRenderer::SetDemoParticle(this);
	}

}