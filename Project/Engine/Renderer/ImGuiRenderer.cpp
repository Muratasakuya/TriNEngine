#include "ImGuiRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Renderer/MeshRenderer.h>
#include <Game/Scenes/Manager/SceneManager.h>
#include <Game/Editor/Manager/EditorManager.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>
#include <Engine/Asset/Asset.h>

//============================================================================*/
//	ImGuiRenderer classMethods
//============================================================================*/

bool ImGuiRenderer::show_ = true;
bool ImGuiRenderer::cameraInfoEnable_ = false;
D3D12_GPU_DESCRIPTOR_HANDLE ImGuiRenderer::renderTextureGPUHandle_ = {};

void ImGuiRenderer::Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle) {

	// RenderTextureのGpuHandleの取得
	renderTextureGPUHandle_ = renderTextureGPUHandle;

}

void ImGuiRenderer::Render() {
#ifdef _DEBUG

	const ImVec2 mainWindowPos = ImVec2(210.0f, 64.0f);
	const ImVec2 mainWindowSize = ImVec2(768.0f, 432.0f);

	if (!show_) {
		ImGui::SetNextWindowPos(ImVec2(6.0f, 24.0f), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(160.0f, 40.0f), ImGuiCond_Always);
		ImGui::Begin("Game", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

		if (ImGui::Button("Display ImGui", ImVec2(144.0f, 24.0f))) {

			show_ = true;
		}

		ImGui::End();
		return;
	}

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(kWindowWidth, kWindowHeight));
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Engine", nullptr, windowFlags);

	//* GuiSetting
	GuiSetting(mainWindowPos);
	//* MainWindow
	MainWindowSetting(mainWindowPos);
	//* GameObjectList
	GameObjectList(mainWindowPos);
	//* CameraList
	CameraList(mainWindowPos);
	//* EditorList
	EditorList(mainWindowPos);
	//* EngineLog
	EngineLog(mainWindowPos, mainWindowSize);
	//* Selected
	SelectedInfo(mainWindowPos, mainWindowSize);
	//* CollisionLog
	CollisionLog(mainWindowPos, mainWindowSize);

	ImGui::End();

#endif // _DEBUG
}

void ImGuiRenderer::GuiSetting(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, 18.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	if (ImGui::Button("UnDisplay ImGui", ImVec2(144.0f, 22.0f))) {
		show_ = false;
	}
	ImGui::PopStyleVar();

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x - 54.0f, 6.0f));
	ImGui::BeginChild("Information",
		ImVec2(768.0f, 44.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::EndChild();

	ImVec2 p1 = ImGui::GetCursorScreenPos();         //* 始点
	p1.y = 56.0f;
	ImVec2 p2 = ImVec2(p1.x + 968.0f - 54.0f, p1.y); //* 終点

	ImGui::GetWindowDrawList()->AddLine(p1, p2, IM_COL32(64, 64, 64, 255), 1.0f);

}

void ImGuiRenderer::MainWindowSetting(const ImVec2& mainWindowPos) {

	const ImVec2 imageSize(768.0f, 432.0f);
	ImGui::SetCursorPos(ImVec2(mainWindowPos.x - 54.0f, mainWindowPos.y));
	ImGui::Image(ImTextureID(renderTextureGPUHandle_.ptr), imageSize);

}

void ImGuiRenderer::GameObjectList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(2.0f, mainWindowPos.y - 6.0f));
	MeshRenderer::SelectGameObject(mainWindowPos);

}

void ImGuiRenderer::CameraList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 128.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));
	if (ImGui::Button("CameraSetting", ImVec2(144.0f, 22.0f))) {

		cameraInfoEnable_ = true;
	}
	ImGui::PopStyleVar();

}

void ImGuiRenderer::EditorList(const ImVec2& mainWindowPos) {

	ImGui::SetCursorPos(ImVec2(6.0f, mainWindowPos.y + 192.0f));
	EditorManager::SelectEditor(mainWindowPos);

}

void ImGuiRenderer::EngineLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(8.0f, mainWindowPos.y + mainWindowSize.y + 4.0f));
	ImGui::BeginChild("EngineChild",
		ImVec2((((kWindowWidth / 2.0f) + (mainWindowSize.x / 2.0f) - 254.0f)) / 2.0f, 212.0f),
		true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::Text("Engine");
	ImGui::Separator();
	ImGui::Text("Frame Rate:       %.1f fps", ImGui::GetIO().Framerate);       //* フレームレート情報
	ImGui::Text("Delta Time:       %.3f s", GameTimer::GetDeltaTime());       //* ΔTime
	ImGui::Text("ScaledDelta Time: %.3f s", GameTimer::GetScaledDeltaTime()); //* ScaledΔTime
	ImGui::Text("DeltaTime Scale:  %.3f", GameTimer::GetTimeScale());         //* TimeScale

	ImGui::EndChild();

}

void ImGuiRenderer::SelectedInfo(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x + mainWindowSize.x + 6.0f - 54.0f, 6.0f));
	ImGui::BeginChild("RightPanelChild", ImVec2(342.0f, mainWindowSize.y + 56.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	if (MeshRenderer::GetSelectedObject()) {

		MeshRenderer::SelectedImGui();

	} else if (cameraInfoEnable_) {

		ImGui::Separator();

	} else if (EditorManager::GetSelectedEditor()) {

		EditorManager::SelectedImGui();

	}

	ImGui::EndChild();

}

void ImGuiRenderer::CollisionLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize) {

	ImGui::SetCursorPos(ImVec2(mainWindowPos.x + mainWindowSize.x - 192.0f, mainWindowPos.y + mainWindowSize.y + 4.0f));
	ImGui::BeginChild("CollisionLogsChild", ImVec2(486.0f, 212.0f), true, ImGuiWindowFlags_AlwaysUseWindowPadding);

	ImGui::EndChild();

}