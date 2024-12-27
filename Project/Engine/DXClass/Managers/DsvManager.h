#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <cstdint>

//============================================================================*/
//	DsvManager class
//============================================================================*/
class DsvManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DsvManager() = default;
	~DsvManager() = default;

	void Init();

	uint32_t Allocate();

	void CreateShadow(uint32_t dsvIndex, ID3D12Resource* dsvResource, DXGI_FORMAT format);

	void ClearDepthStencilView(ID3D12GraphicsCommandList* commandList);
	void ClearShadowDepthStencilView(ID3D12GraphicsCommandList* commandList);

	//* getter *//

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	D3D12_CPU_DESCRIPTOR_HANDLE& GetNoramlCPUHandle() { return dsvCPUHandle_; }
	D3D12_CPU_DESCRIPTOR_HANDLE& GetShadowMapCPUHandle() { return dsvShadowMapCPUHandle_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static const uint32_t kMaxDsvCount_;
	uint32_t useIndex_;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;

	ComPtr<ID3D12Resource> resource_;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle_;

	D3D12_CPU_DESCRIPTOR_HANDLE dsvShadowMapCPUHandle_;

	uint32_t descriptorSize_;

	//===================================================================*/
	///* functions

	void MakeDescriptorHeap();

	void CreateDSV();

	ComPtr<ID3D12Resource> MakeDepthResource(
		DXGI_FORMAT resourceFormat, DXGI_FORMAT depthClearFormat);

	bool CanAllocate();

};