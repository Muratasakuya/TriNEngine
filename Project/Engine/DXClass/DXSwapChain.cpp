#include "DXSwapChain.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	DXSwapChain classMethods
//============================================================================*/

void DXSwapChain::Init(HWND hwnd, ID3D12CommandQueue* commandQueue) {

	resources_.resize(kBufferCount, nullptr);

	swapChain_ = nullptr;
	desc_.Width = kWindowWidth;
	desc_.Height = kWindowHeight;
	desc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc_.SampleDesc.Count = 1;
	desc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc_.BufferCount = kBufferCount;
	desc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	HRESULT hr = GraphicsEngine::GetDevice()->GetDxgiFactory()->CreateSwapChainForHwnd(
		commandQueue, hwnd, &desc_, nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain_.GetAddressOf()));
	assert(SUCCEEDED(hr));

	for (uint32_t index = 0; index < kBufferCount; ++index) {

		hr = swapChain_->GetBuffer(index, IID_PPV_ARGS(&resources_[index]));
		assert(SUCCEEDED(hr));

		resources_[index]->SetName((L"backBufferResource" + std::to_wstring(index)).c_str());
	}

}