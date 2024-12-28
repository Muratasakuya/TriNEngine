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

	void Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle);

	void Render();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGPUHandle_;

	ImGuiWindowFlags windowFlag_;

	//========================================================================*/
	//* functions

	void MainWindow();

	void EngineView();

	void GameView();

	void InspectorView();

};