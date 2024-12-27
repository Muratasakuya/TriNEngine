#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>

// c++
#include <array>
#include <cassert>

//============================================================================*/
//	DXRootSignature class
//============================================================================*/
class DXRootSignature {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXRootSignature() = default;
	~DXRootSignature() = default;

	void Create(const PostProcessPipelineType& pipelineType);
	void Create(const RendererPipelineType& pipelineType);
	void Create(const ShadowPipelineType& pipelineType);
	void Create(const ComputePipelineType& pipelineType);

	//* getter *//

	ID3D12RootSignature* GetPostProcess(PostProcessPipelineType pipelineType) const { return postProcessRootSignature_[pipelineType].Get(); }

	ID3D12RootSignature* GetRenderer(RendererPipelineType pipelineType) const { return rendererRootSignature_[pipelineType].Get(); }

	ID3D12RootSignature* GetShadow(ShadowPipelineType pipelineType) const { return shadowRootSignature_[pipelineType].Get(); }

	ID3D12RootSignature* GetCompute(ComputePipelineType pipelineType) const { return computeRootSignature_[pipelineType].Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<ID3DBlob> signatureBlob_;
	ComPtr<ID3DBlob> errorBlob_;

	//* postProcess *//

	std::array<ComPtr<ID3D12RootSignature>, postProcessPipelineNum> postProcessRootSignature_;

	//* renderer *//

	std::array<ComPtr<ID3D12RootSignature>, rendererPipelineNum> rendererRootSignature_;

	//* shadow *//

	std::array<ComPtr<ID3D12RootSignature>, shadowPipelineNum> shadowRootSignature_;

	//* compute *//

	std::array<ComPtr<ID3D12RootSignature>, computePipelineNum> computeRootSignature_;

};