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
//	OffscreenRenderer class
//============================================================================*/
class OffscreenRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	OffscreenRenderer() = default;
	~OffscreenRenderer() = default;

	void Init(SrvManager* srvManager, RtvManager* rtvManager);

	//* getter *//

	ID3D12Resource* GetRenderTexture() const { return renderTextureResource_.Get(); }
	ID3D12Resource* GetGuiTexture() const { return guiTextureResource_.Get(); }

	D3D12_GPU_DESCRIPTOR_HANDLE GetRenderTextureGPUHandle() const { return renderTextureGpuHandle_; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetGuiGPUHandle() const { return guiTextureGpuHandle_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Resource> renderTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE renderTextureGpuHandle_;

	ComPtr<ID3D12Resource> guiTextureResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE guiTextureGpuHandle_;

	//========================================================================*/
	//* function

	ComPtr<ID3D12Resource> CreateTextureResource();

};