#include "DemoEditorScene.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	DemoEditorScene classMethods
//============================================================================*/

void DemoEditorScene::Init() {

	// model
	Asset::LoadModel("./Resources/Model/Obj/CG/", "plane.obj");
	Asset::LoadModel("./Resources/Model/Obj/Particle/", "hitLine.obj");
	// texture
	Asset::LoadTexture("uvChecker");
	Asset::LoadTexture("circle");
	Asset::LoadTexture("circle4");
	Asset::LoadTexture("starCircle");
	
	particleEditor_ = std::make_unique<ParticleEditor>();

}

void DemoEditorScene::Update() {

	particleEditor_->Update();

}