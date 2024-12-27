#include "GameTimer.h"

//============================================================================*/
//	GameTimer classMethods
//============================================================================*/

std::chrono::steady_clock::time_point GameTimer::lastFrameTime_ = std::chrono::steady_clock::now();
float GameTimer::deltaTime_ = 0.0f;
float GameTimer::timeScale_ = 1.0f;

void GameTimer::Update() {

	auto currentFrameTime = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsedTime = currentFrameTime - lastFrameTime_;
	deltaTime_ = elapsedTime.count();

	lastFrameTime_ = currentFrameTime;
}