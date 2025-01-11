#include "TestParticle.h"

//============================================================================*/
//	include
//============================================================================*/

//============================================================================*/
//	TestParticle classMethods
//============================================================================*/

void TestParticle::Init() {

	BaseParticle::SetParticleRenderer("test");
	BaseParticle::Create("hitLine.obj");

	//* 固定設定 *//

	blendMode_ = kBlendModeAdd;        // 加算
	parameter_.isUniform = true;       // 均一に出る
	parameter_.isUseBillboard = false; // Billboard無効

}

void TestParticle::Update() {

	particleSystem_->Update();

}