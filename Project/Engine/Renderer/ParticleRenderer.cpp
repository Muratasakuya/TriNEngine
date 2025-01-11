#include "ParticleRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Renderer/MeshRenderer.h>
#include <Engine/Renderer/SpriteRenderer.h>
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	ParticleRenderer classMethods
//============================================================================*/

std::vector<IBaseParticle*> ParticleRenderer::particles_ = {};
std::vector<IBaseParticle*> ParticleRenderer::demoParticles_ = {};

void ParticleRenderer::Render() {

	for (const auto& particle : particles_) {

		particle->Draw();
	}
}

void ParticleRenderer::RenderDemo() {

	for (const auto& particle : demoParticles_) {

		particle->Draw();
	}
}

void ParticleRenderer::SetParticle(IBaseParticle* particle) {

	if (!particle) {
		return;
	}

	particles_.emplace_back(particle);
}

void ParticleRenderer::SetDemoParticle(IBaseParticle* demoParticle) {

	if (!demoParticle) {
		return;
	}

	demoParticles_.emplace_back(demoParticle);
}

void ParticleRenderer::EraseParticle(IBaseParticle* particle) {

	particles_.erase(std::remove(particles_.begin(), particles_.end(), particle), particles_.end());
	demoParticles_.erase(std::remove(demoParticles_.begin(), demoParticles_.end(), particle), demoParticles_.end());
}

void ParticleRenderer::Clear() {

	particles_.clear();
	demoParticles_.clear();
}