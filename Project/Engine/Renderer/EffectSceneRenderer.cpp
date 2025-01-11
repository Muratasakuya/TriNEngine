#include "EffectSceneRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	EffectSceneRenderer classMethods
//============================================================================*/

ComPtr<ID3D12Resource> EffectSceneRenderer::CreateTextureResource() {

	// RenderTargetで設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = kWindowWidth;                            // 横幅
	resourceDesc.Height = kWindowHeight;                          // 縦幅
	resourceDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;        // フォーマット設定
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET; // RenderTargetとして利用
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;  // 2Dテクスチャで設定
	resourceDesc.SampleDesc.Count = 1;                            // サンプリングカウント。1固定
	resourceDesc.DepthOrArraySize = 1;                            // 配列サイズまたは深度を1に設定
	resourceDesc.MipLevels = 1;                                   // 1で設定、しなくてもdefaultで1になるらしい

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// 色
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearValue.Color[0] = kWindowClearColor.r;
	clearValue.Color[1] = kWindowClearColor.g;
	clearValue.Color[2] = kWindowClearColor.b;
	clearValue.Color[3] = kWindowClearColor.a;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = GraphicsEngine::GetDevice()->Get()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));

	return resource;
}

void EffectSceneRenderer::Init(SrvManager* srvManager, RtvManager* rtvManager) {

	// renderTexture作成
	renderTextureResource_ = CreateTextureResource();
	renderTextureResource_->SetName(L"effectSceneRenderTexture");

	// RTVの作成
	uint32_t rtvIndex = rtvManager->Allocate();
	rtvManager->Create(rtvIndex, renderTextureResource_.Get());

	// SRVの作成
	uint32_t srvIndex = srvManager->Allocate();
	renderTextureGpuHandle_ = srvManager->GetGPUHandle(srvIndex);
	srvManager->CreateSRVForTexture2D(srvIndex, renderTextureResource_.Get(), DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 1);

}