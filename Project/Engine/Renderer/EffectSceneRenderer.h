#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// front
class SrvManager;
class RtvManager;

//============================================================================*/
//	EffectSceneRenderer class
//============================================================================*/
class EffectSceneRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	EffectSceneRenderer() = default;
	~EffectSceneRenderer() = default;

	void Init(SrvManager* srvManager, RtvManager* rtvManager);

	//* getter *//

	ID3D12Resource* GetRenderTexture() const { return renderTextureResource_.Get(); }

	D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTextureGPUHandle() const { return renderTextureGpuHandle_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Resource> renderTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGpuHandle_;

	//========================================================================*/
	//* function

	ComPtr<ID3D12Resource> CreateTextureResource();

};