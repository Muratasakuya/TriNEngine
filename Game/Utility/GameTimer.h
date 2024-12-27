#pragma once

//============================================================================*/
//	include
//============================================================================*/

// c++
#include <cstdint>
#include <chrono>

//============================================================================*/
//	GameTimer class
//============================================================================*/
class GameTimer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	GameTimer() = default;
	~GameTimer() = default;

	static void Update();

	//* getter *//

	static float GetDeltaTime() { return deltaTime_; }
	static float GetScaledDeltaTime() { return deltaTime_ * timeScale_; }

	static float GetTimeScale() { return timeScale_; }

	//* setter *//

	static float SetTimeScale(float timeScale) { timeScale_ = timeScale; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static std::chrono::steady_clock::time_point lastFrameTime_;

	static float deltaTime_;

	static float timeScale_;

};