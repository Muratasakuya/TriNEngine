#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Quaternion.h>

// imgui
#include <imgui.h>

// directX
#include <d3d12.h>

// c++
#include <string>
#include <functional>

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

	void Init(const D3D12_GPU_DESCRIPTOR_HANDLE& renderTextureGPUHandle,
		const D3D12_GPU_DESCRIPTOR_HANDLE& demoSceneRenderTextureGPUHandle);

	void Render();

	//* task *//

	void RenderTask();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGPUHandle_;
	D3D12_GPU_DESCRIPTOR_HANDLE demoSceneRenderTextureGPUHandle_;

	ImGuiWindowFlags windowFlag_;

	//========================================================================*/
	//* functions

	void MainWindow();

	void EngineView();

	void GameView();

	void InspectorView();

	//* task *//

	void DisplayMatrix(const std::string& windowName, const Matrix4x4& matrix);

	void DisplayQuaternion(const std::string& windowName, const Quaternion& quaternion);

};