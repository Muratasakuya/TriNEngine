#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>

// directX
#include <d3d12.h>

// c++
#include <cstdint>

// front
class SrvManager;
class RtvManager;
class DsvManager;

//============================================================================*/
//	ShadowMapRenderer class
//============================================================================*/
class ShadowMapRenderer {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ShadowMapRenderer() = default;
	~ShadowMapRenderer() = default;

	void Init(SrvManager* srvManager, DsvManager* dsvManager);

	//* getter *//

	ID3D12Resource* GetShadowResource() const { return shadowMapResource_.Get(); }

	D3D12_GPU_DESCRIPTOR_HANDLE GetShadowGPUHandle() const { return shadowMapGpuHandle_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3D12Resource> shadowMapResource_;
	D3D12_GPU_DESCRIPTOR_HANDLE shadowMapGpuHandle_;

	//========================================================================*/
	//* function

	void CreateShadowMapResource();

};