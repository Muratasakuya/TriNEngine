#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <cstdint>
#include <cassert>

//============================================================================*/
//	RtvManager class
//============================================================================*/
class RtvManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	RtvManager() = default;
	~RtvManager() = default;

	void Init();

	void Create(uint32_t index, ID3D12Resource* resource);

	uint32_t Allocate();
	void Reset();

	void BeginOffscreenSetRenderTargets(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle);
	void SetRenderTargets(ID3D12GraphicsCommandList* commandList, UINT backBufferIndex);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static const uint32_t kMaxRtvCount_;
	uint32_t useIndex_;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	D3D12_RENDER_TARGET_VIEW_DESC desc_;

	uint32_t descriptorSize_;

	//========================================================================*/
	//* functions

	void MakeDescriptorHeap();

	bool CanAllocate();

};