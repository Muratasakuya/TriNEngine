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

	static void SetParticle(IBaseParticle* particle);

	static void EraseParticle(IBaseParticle* particle);

	static void Clear();

	//* imgui *//

	static void SelectParticle();

	static void SelectedImGui();

	//* getter *//

	static IBaseParticle* GetSelectedParticle() { return selectedParticle_; };

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//===================================================================*/
	///* variables

	static std::vector<IBaseParticle*> particles_;

	static IBaseParticle* selectedParticle_;

	static int currentParticleIndex_;

};