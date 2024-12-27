#include "RtvManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	RtvManager classMethods
//============================================================================*/

const uint32_t RtvManager::kMaxRtvCount_ = 6;

void RtvManager::MakeDescriptorHeap() {

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = kMaxRtvCount_;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = GraphicsEngine::GetDevice()->Get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap_));
	assert(SUCCEEDED(hr));
}

void RtvManager::Init() {

	useIndex_ = kBufferCount;

	// Descの設定
	desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;        // 出力結果をSRGBも変換して書き込む
	desc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;   // 2dテクスチャとして書き込む

	// Descriptor生成
	MakeDescriptorHeap();
	descriptorSize_ = GraphicsEngine::GetDevice()->Get()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

}

void RtvManager::Create(uint32_t index, ID3D12Resource* resource) {

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GetCPUHandle(index);
	GraphicsEngine::GetDevice()->Get()->CreateRenderTargetView(resource, &desc_, handle);
}

uint32_t RtvManager::Allocate() {

	if (!CanAllocate()) {
		throw std::runtime_error("Cannot allocate more RTVs, maximum count reached.");
	}

	int index = useIndex_;
	useIndex_++;

	return index;
}

bool RtvManager::CanAllocate() {
	return useIndex_ < kMaxRtvCount_;
}

void RtvManager::Reset() {

	useIndex_ = kBufferCount;
}

void RtvManager::BeginOffscreenSetRenderTargets(
	ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvCPUHandle) {

	uint32_t rtvIndex = Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUHandle(rtvIndex);
	commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvCPUHandle);
	float kRenderTargetClearColor[] =
	{ kWindowClearColor.r, kWindowClearColor.g, kWindowClearColor.b, kWindowClearColor.a };
	commandList->ClearRenderTargetView(rtvHandle, kRenderTargetClearColor, 0, nullptr);

}

void RtvManager::SetRenderTargets(
	ID3D12GraphicsCommandList* commandList, UINT backBufferIndex) {

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = GetCPUHandle(backBufferIndex);
	commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	float kRenderTargetClearColor[] =
	{ kWindowClearColor.r, kWindowClearColor.g, kWindowClearColor.b, kWindowClearColor.a };
	commandList->ClearRenderTargetView(rtvHandle, kRenderTargetClearColor, 0, nullptr);

}

D3D12_CPU_DESCRIPTOR_HANDLE RtvManager::GetCPUHandle(uint32_t index) {

	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap_.Get()->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize_ * index);

	return handleCPU;
}