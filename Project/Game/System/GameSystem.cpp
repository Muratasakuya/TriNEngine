#include "GameSystem.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/3D/PrimitiveDrawer.h>
#include <Game/Utility/GameTimer.h>

//============================================================================*/
//	GameSystem classMethods
//============================================================================*/

std::unique_ptr<CollisionManager> GameSystem::collisionManager_ = nullptr;
std::unique_ptr<CameraManager> GameSystem::cameraManager_ = nullptr;
std::unique_ptr<LightManager> GameSystem::lightManager_ = nullptr;

void GameSystem::Init() {

	collisionManager_ = std::make_unique<CollisionManager>();

	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Init();

	lightManager_ = std::make_unique<LightManager>();
	lightManager_->Init();

}

void GameSystem::Update() {

	GameTimer::Update();

	Input::GetInstance()->Update();

	PrimitiveDrawer::GetInstance()->Update();

	collisionManager_->UpdateAllCollisions();

	cameraManager_->Update();

	lightManager_->Update();

}

void GameSystem::Finalize() {

	collisionManager_.reset();
	cameraManager_.reset();
	lightManager_.reset();

}

void GameSystem::Reset() {

	PrimitiveDrawer::GetInstance()->Reset();
}
