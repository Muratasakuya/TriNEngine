#pragma once

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

// directX
#include <d3d12.h>

//============================================================================*/
//	ImGuiRenderer class
//============================================================================*/
class ImGuiRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ImGuiRenderer() = default;
	~ImGuiRenderer() = default;

	static void Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle);

	static void Render();

	//========================================================================*/
	//* variables

	static bool cameraInfoEnable_;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static bool show_;

	static D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGPUHandle_;

	//========================================================================*/
	//* functions

	static void GuiSetting(const ImVec2& mainWindowPos);

	static void MainWindowSetting(const ImVec2& mainWindowPos);

	static void GameObjectList(const ImVec2& mainWindowPos);

	static void CameraList(const ImVec2& mainWindowPos);

	static void EditorList(const ImVec2& mainWindowPos);

	static void EngineLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize);

	static void SelectedInfo(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize);

	static void CollisionLog(const ImVec2& mainWindowPos, const ImVec2& mainWindowSize);

};