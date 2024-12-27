#include "ShadowMapRenderer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	ShadowMapRenderer classMethods
//============================================================================*/

void ShadowMapRenderer::CreateShadowMapResource() {

	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = kShadowMapWidth;                         // シャドウマップの幅
	resourceDesc.Height = kShadowMapHeight;                       // シャドウマップの高さ
	resourceDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;

	// 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = DXGI_FORMAT_D32_FLOAT;
	clearValue.DepthStencil.Depth = 1.0f;

	HRESULT hr = GraphicsEngine::GetDevice()->Get()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&clearValue,
		IID_PPV_ARGS(&shadowMapResource_)
	);
	assert(SUCCEEDED(hr));
	shadowMapResource_->SetName(L"shadowMapTexture");

}

void ShadowMapRenderer::Init(SrvManager* srvManager, DsvManager* dsvManager) {

	// DXGI_FORMAT_D32_FLOAT

	// 深度テクスチャ作成
	CreateShadowMapResource();

	uint32_t dsvIndex = dsvManager->Allocate();
	dsvManager->CreateShadow(dsvIndex, shadowMapResource_.Get(), DXGI_FORMAT_D32_FLOAT);

	// SRV作成
	uint32_t srvIndex = srvManager->Allocate();
	shadowMapGpuHandle_ = srvManager->GetGPUHandle(srvIndex);
	srvManager->CreateSRVForTexture2D(srvIndex, shadowMapResource_.Get(), DXGI_FORMAT_R32_FLOAT, 1);

}