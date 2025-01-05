#include "TriNFramework.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Asset/Asset.h>
#include <Engine/Renderer/ImGuiRenderer.h>
#include <Engine/Process/Input.h>
#include <Game/3D/PrimitiveDrawer.h>

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
	sceneManager_ = std::make_unique<SceneManager>("Title");

#ifdef _DEBUG
	imGuiRenderer_ = std::make_unique<ImGuiRenderer>();
	imGuiRenderer_->Init(graphicsEngine_->GetGuiTextureGPUHandle());
#endif // _DEBUG

}

void TriNFramework::Update() {

	sceneManager_->InitNextScene();

	graphicsEngine_->BeginRenderFrame();

	gameSystem_->Update();
	imGuiRenderer_->Render();

	sceneManager_->Update();

}

void TriNFramework::Draw() {

	graphicsEngine_->Render();

	graphicsEngine_->EndRenderFrame();

	gameSystem_->Reset();

	sceneManager_->SwitchScene();

}

void TriNFramework::Finalize() {

	graphicsEngine_->Finalize();
	Asset::Finalize();
	PrimitiveDrawer::Finalize();
	Input::Finalize();

	graphicsEngine_.reset();

	sceneManager_->Finalize();
	sceneManager_.reset();

	gameSystem_->Finalize();
	gameSystem_.reset();

}