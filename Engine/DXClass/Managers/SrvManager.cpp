#include "SrvManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	SrvManager classMethods
//============================================================================*/

const uint32_t SrvManager::kMaxSrvCount_ = 256;

void SrvManager::MakeDescriptorHeap() {

	D3D12_DESCRIPTOR_HEAP_DESC desc{};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = kMaxSrvCount_;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = GraphicsEngine::GetDevice()->Get()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(hr));
}

void SrvManager::Init() {

	useIndex_ = 0;

	// descripptorHeapの作成
	MakeDescriptorHeap();
	descriptorSize_ = GraphicsEngine::GetDevice()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

}

void SrvManager::SetDescriptorHeaps(ID3D12GraphicsCommandList* commandList) {

	ID3D12DescriptorHeap* descriptorHeaps[] = { descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(1, descriptorHeaps);
}

uint32_t SrvManager::Allocate() {

	if (!CanAllocate()) {
		throw std::runtime_error("Cannot allocate more SRVs, maximum count reached.");
	}

	int index = useIndex_;
	useIndex_++;

	return index;
}

bool SrvManager::CanAllocate() {
	return useIndex_ < kMaxSrvCount_;
}

D3D12_CPU_DESCRIPTOR_HANDLE SrvManager::GetCPUHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SrvManager::GetGPUHandle(uint32_t index) {

	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap_.Get()->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize_ * index);

	return handleGPU;
}


void SrvManager::CreateSRVForTexture2D(
	uint32_t srvIndex, ID3D12Resource* pResource, DXGI_FORMAT format, UINT mipLevels) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = mipLevels;

	GraphicsEngine::GetDevice()->Get()->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandle(srvIndex));
}

void SrvManager::CreateSRVForStructureBuffer(uint32_t srvIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	srvDesc.Buffer.NumElements = numElements;
	srvDesc.Buffer.StructureByteStride = structureByteStride;

	GraphicsEngine::GetDevice()->Get()->CreateShaderResourceView(pResource, &srvDesc, GetCPUHandle(srvIndex));
}

void SrvManager::CreateUAVForStructureBuffer(uint32_t uavIndex, ID3D12Resource* pResource, UINT numElements, UINT structureByteStride) {

	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc{};
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.CounterOffsetInBytes = 0;
	uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	uavDesc.Buffer.NumElements = numElements;
	uavDesc.Buffer.StructureByteStride = structureByteStride;

	GraphicsEngine::GetDevice()->Get()->CreateUnorderedAccessView(pResource, nullptr, &uavDesc, GetCPUHandle(uavIndex));
}