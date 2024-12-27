#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Collision/Managers/CollisionManager.h>
#include <Game/Camera/Manager/CameraManager.h>
#include <Game/Light/Manager/LightManager.h>

// c++
#include <cstdint>
#include <memory>

//============================================================================*/
//	GameSystem class
//============================================================================*/
class GameSystem {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GameSystem() = default;
	~GameSystem() = default;

	void Init();

	void Update();

	//* getter *//

	static CollisionManager* GetCollision() { return collisionManager_.get(); }

	static CameraManager* GameCamera() { return cameraManager_.get(); }

	static LightManager* GameLight() { return lightManager_.get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::unique_ptr<CollisionManager> collisionManager_;

	static std::unique_ptr<CameraManager> cameraManager_;

	static std::unique_ptr<LightManager> lightManager_;

};