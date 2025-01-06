#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/BaseParticle.h>

//============================================================================*/
//	TestParticle class
//============================================================================*/
class TestParticle
	:public BaseParticle<kDispersion> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TestParticle() = default;
	~TestParticle() = default;

	void Init() override;

	void Update() override;

	void DerivedImGui() override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables


};