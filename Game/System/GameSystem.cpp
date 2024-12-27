#include "GameSystem.h"

//============================================================================*/
//	include
//============================================================================*/

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

	collisionManager_->UpdateAllCollisions();

	cameraManager_->Update();

	lightManager_->Update();

}