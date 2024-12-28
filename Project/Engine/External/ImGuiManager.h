#pragma once

//============================================================================*/
//	include
//============================================================================*/

// windows
#include <Windows.h>

// imgui
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>

// c++
#include <cstdint>

// front
class SrvManager;

//============================================================================*/
//	ImGuiManager class
//============================================================================*/
class ImGuiManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ImGuiManager() = default;
	~ImGuiManager() = default;

	void Init(HWND hwnd, UINT bufferCount, SrvManager* srvManager);

	void Begin();
	void End();

	void Draw(ID3D12GraphicsCommandList* commandList);

	void Finalize();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	SrvManager* srvManager_ = nullptr;

};