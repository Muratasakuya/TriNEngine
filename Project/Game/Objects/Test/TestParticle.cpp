#include "TestParticle.h"

//============================================================================*/
//	include
//============================================================================*/

//============================================================================*/
//	TestParticle classMethods
//============================================================================*/

void TestParticle::Init() {

	BaseParticle::SetParticleRenderer("test");

	parameter_.scale.uniform = Vector3(1.0f, 1.0f, 1.0f);

	parameter_.count = 8;
	parameter_.frequency = 0.1f;

	BaseParticle::Create("plane.obj");
	particleSystem_->SetTexture(GetName(), "circle");

	blendMode_ = kBlendModeAdd;

}

void TestParticle::Update() {

	particleSystem_->Update();

}

void TestParticle::DerivedImGui() {


}