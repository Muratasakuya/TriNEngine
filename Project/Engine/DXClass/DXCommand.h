#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>
#include <dxgi1_6.h>

// c++
#include <cstdint>
#include <chrono>
#include <thread>
#include <cassert>

//============================================================================*/
//	DXCommand class
//============================================================================*/
class DXCommand {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXCommand() = default;
	~DXCommand() = default;

	void Init(ID3D12Device* device);

	void Execute(IDXGISwapChain4* swapChain);

	void WaitForGPU();

	void TransitionBarrier(
		ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);
	void CopyRenderTexture(ID3D12Resource* dstResource, D3D12_RESOURCE_STATES dstState,
		ID3D12Resource* srcResource, D3D12_RESOURCE_STATES srcState);

	void Finalize(HWND hwnd);

	//* getter *//

	ID3D12CommandQueue* GetQueue() const { return commandQueue_.Get(); }

	ID3D12GraphicsCommandList* GetCommandList() const { return commandList_.Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::chrono::steady_clock::time_point reference_;

	ComPtr<ID3D12CommandQueue> commandQueue_;
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc_;

	ComPtr<ID3D12CommandAllocator> commandAllocator_;
	ComPtr<ID3D12GraphicsCommandList> commandList_;

	ComPtr<ID3D12Fence> fence_;
	uint64_t fenceValue_;
	HANDLE fenceEvent_;

	//========================================================================*/
	//* function

	void UpdateFixFPS();

};