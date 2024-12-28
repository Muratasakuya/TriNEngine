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

	static D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGPUHandle_;

	static ImGuiWindowFlags windowFlag_;

	static bool testBool_;
	static int testInt_;
	static float testFloat_;

	//========================================================================*/
	//* functions

	static void MainWindow();

	static void EngineView();

	static void GameView();

	static void InspectorView();

};