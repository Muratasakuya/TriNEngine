#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>

//============================================================================*/
//	DXDepthRaster class
//============================================================================*/
class DXDepthRaster {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXDepthRaster() = default;
	~DXDepthRaster() = default;

	void Create();
	void Create(const RendererPipelineType& pipelineType);
	void Create(const ShadowPipelineType& pipelineType);

	//* getter *//

	D3D12_RASTERIZER_DESC GetRasterizerDesc() const { return rasterizerDesc_; }

	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const { return depthStencilDesc_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	D3D12_RASTERIZER_DESC rasterizerDesc_;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_;

};