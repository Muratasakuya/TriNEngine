#pragma once
#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/IBaseParticle.h>

// imgui
#include <imgui.h>

// c++
#include <vector>
#include <unordered_map>

//============================================================================*/
//	ParticleRenderer class
//============================================================================*/
class ParticleRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleRenderer() = default;
	~ParticleRenderer() = default;

	static void Render();

	static void RenderDemo();

	static void SetParticle(IBaseParticle* particle);
	static void SetDemoParticle(IBaseParticle* demoParticle);

	static void EraseParticle(IBaseParticle* particle);

	static void Clear();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<IBaseParticle*> particles_;
	static std::vector<IBaseParticle*> demoParticles_;

};