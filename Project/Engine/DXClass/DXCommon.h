#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Lib/MathUtils/Vector4.h>

// windows
#include <Windows.h>

// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

// c++
#include <memory>
#include <chrono>
#include <thread>

//============================================================================*/
//	DXCommon class
//============================================================================*/
class DXCommon {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXCommon() = default;
	~DXCommon() = default;

	void Init();

	void Finalize(HWND hwnd);

	void DebugLayer();
	void DebugInfo();

	void SetViewportAndScissor(uint32_t width, uint32_t height);

	void Execute(IDXGISwapChain4* swapChain);

	void WaitForGPU();

	//* getter *//

	ID3D12CommandQueue* GetCommandQueue() const { return commandQueue_.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

	IDxcUtils* GetDxcUtils() const { return dxcUtils_.Get(); }

	IDxcCompiler3* GetDxcCompiler() const { return dxcCompiler_.Get(); }

	IDxcIncludeHandler* GetIncludeHandler() const { return includeHandler_.Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Color clearColor_;

	std::chrono::steady_clock::time_point reference_;

	ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	ComPtr<ID3D12CommandQueue> commandQueue_;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_;

	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;

	IDXGISwapChain4* swapChain_;

	D3D12_VIEWPORT viewport_;
	D3D12_RECT scissorRect_;

	//========================================================================*/
	//* functions

	void CreateFenceEvent();

	void InitDXCompiler();

	void CreateCommand();

	void UpdateFixFPS();

};