#include "TriNFramework.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Game/3D/PrimitiveDrawer.h>
#include <Game/Utility/GameTimer.h>

//============================================================================*/
//	TriNFramework classMethods
//============================================================================*/

void TriNFramework::Run() {

	Init();

	while (true) {

		Update();

		Draw();

		if (graphicsEngine_->ProcessMessage()) {
			break;
		}
	}

	Finalize();

}

void TriNFramework::Init() {

	// engine
	graphicsEngine_ = std::make_unique<GraphicsEngine>();
	graphicsEngine_->Init();
	// asset
	Asset::Init();
	// lineDrawer
	PrimitiveDrawer::GetInstance()->Init(graphicsEngine_->GetPipeline());
	// gameSystem
	gameSystem_ = std::make_unique<GameSystem>();
	gameSystem_->Init();
	// scene
	sceneManager_ = std::make_unique<SceneManager>("Game");

}

void TriNFramework::Update() {
	graphicsEngine_->BeginRenderFrame();
	GameTimer::Update();

	gameSystem_->Update();
	ImGuiRenderer::Render();

	sceneManager_->Update();

}

void TriNFramework::Draw() {

	graphicsEngine_->Render();

	graphicsEngine_->EndRenderFrame();

	sceneManager_->SwitchScene();

}

void TriNFramework::Finalize() {

	graphicsEngine_->Finalize();

	graphicsEngine_.reset();
	gameSystem_.reset();
	sceneManager_.reset();

}