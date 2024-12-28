#include "GameTimer.h"

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

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

void GameTimer::ImGui() {

	ImGui::Text("Frame Rate:       %.1f fps", ImGui::GetIO().Framerate); //* フレームレート情報
	ImGui::Text("Delta Time:       %.3f s", deltaTime_);                 //* ΔTime
	ImGui::Text("ScaledDelta Time: %.3f s", GetScaledDeltaTime());       //* ScaledΔTime
	ImGui::Text("Time Scale");
	ImGui::DragFloat(" ", &timeScale_, 0.01f);

}