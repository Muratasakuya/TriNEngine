#include "DXDepthRaster.h"

//============================================================================*/
//	DXDepthRaster classMethods
//============================================================================*/

void DXDepthRaster::Create() {

	rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	depthStencilDesc_.DepthEnable = false;
	depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

}

void DXDepthRaster::Create(const RendererPipelineType& pipelineType) {

	if (pipelineType == Object2D) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		// Depth機能無効
		depthStencilDesc_.DepthEnable = false;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (pipelineType == PrimitiveLine) {

		rasterizerDesc_.FillMode = D3D12_FILL_MODE_WIREFRAME;
		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.AntialiasedLineEnable = TRUE;

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	} else if (pipelineType == NormalObject3D || pipelineType == TargetShadowObject3D) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		// Depth機能有効
		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	} else if (pipelineType == NormalParticle) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

		depthStencilDesc_.DepthEnable = true;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	}

}

void DXDepthRaster::Create(const ShadowPipelineType& pipelineType) {

	if (pipelineType == ShadowDepth) {

		rasterizerDesc_.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
		rasterizerDesc_.DepthClipEnable = TRUE;

		depthStencilDesc_.DepthEnable = TRUE;
		depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	}
}