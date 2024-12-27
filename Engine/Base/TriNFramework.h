#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Game/System/GameSystem.h>
#include <Game/Scenes/Manager/SceneManager.h>

// c++
#include <memory>

//============================================================================*/
//	TriNFramework class
//============================================================================*/
class TriNFramework {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	TriNFramework() = default;
	~TriNFramework() = default;

	void Run();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//============================================================================*/
	// LeakChecker
	//============================================================================*/

	struct LeakChecker {

		~LeakChecker() {

			ComPtr<IDXGIDebug1> debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {

				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}
		}
	};
	LeakChecker leakChecker_;

	//========================================================================*/
	//* variables

	std::unique_ptr<GraphicsEngine> graphicsEngine_;

	std::unique_ptr<GameSystem> gameSystem_;

	std::unique_ptr<SceneManager> sceneManager_;

	//========================================================================*/
	//* functions

	void Init();

	void Update();

	void Draw();

	void Finalize();

};