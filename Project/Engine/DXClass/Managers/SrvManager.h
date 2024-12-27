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
//	SrvManager class
//============================================================================*/
class SrvManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	SrvManager() = default;
	~SrvManager() = default;

	void Init();

	uint32_t Allocate();

	void SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList);

	void CreateSRVForTexture2D(uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels);
	void CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);
	void CreateUAVForStructureBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride);

	//* getter *//

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	ID3D12DescriptorHeap* GetDescriptorHeap() const { return descriptorHeap_.Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static const uint32_t kMaxSrvCount_;
	uint32_t useIndex_;

	ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	uint32_t descriptorSize_;

	//===================================================================*/
	///* functions

	void MakeDescriptorHeap();

	bool CanAllocate();

};