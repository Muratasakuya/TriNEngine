#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Camera/Camera2D.h>
#include <Game/Camera/Camera3D.h>
#include <Game/Camera/FollowCamera.h>
#include <Game/Camera/DebugCamera.h>
#include <Game/Camera/DemoDebugCamera.h>
#include <Game/Camera/SunLightCamera.h>

// c++
#include <memory>
#include <unordered_map>

//============================================================================*/
//	CameraManager class
//============================================================================*/
class CameraManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	CameraManager() = default;
	~CameraManager() = default;

	void Init();

	void Update();

	void DrawDebug();

	void SelectGameCamera();

	void ImGui();

	//* getter *//

	Camera2D* GetCamera2D() const { return camera2D_.get(); }

	Camera3D* GetCamera3D() const { return camera3D_.get(); }

	FollowCamera* GetFollowCamera() const { return followCamera_.get(); }

	DemoDebugCamera* GetDemoDebugCamera() const { return demoDebugCamera_.get(); }

	SunLightCamera* GetSunLightCamera() const { return sunLightCamera_.get(); }

	bool SelectedGameCamera() const { return selectGameCamera_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::unique_ptr<Camera2D> camera2D_;

	std::unique_ptr<Camera3D> camera3D_;

	std::unique_ptr<FollowCamera> followCamera_;

	std::unique_ptr<DebugCamera> debugCamera_;

	std::unique_ptr<DemoDebugCamera> demoDebugCamera_;

	std::unique_ptr<SunLightCamera> sunLightCamera_;

	bool debugCameraEnable_;

	bool selectGameCamera_;

};