#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/Pipeline/PipelineTypes.h>
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/DXShaderCompiler.h>
#include <Engine/DXClass/Pipeline/DXRootSignature.h>
#include <Engine/DXClass/Pipeline/DXDepthRaster.h>
#include <Engine/DXClass/Pipeline/DXInputLayout.h>
#include <Engine/DXClass/Pipeline/DXBlendState.h>

// directX
#include <d3d12.h>

// c++
#include <memory>
#include <array>
#include <cassert>

//============================================================================*/
//	PipelineManager class
//============================================================================*/
class PipelineManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PipelineManager() = default;
	~PipelineManager() = default;

	void Create(ID3D12Device* device);

	//* command *//

	void SetPostProcessPipeline(ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType);
	void SetRendererPipeline(ID3D12GraphicsCommandList* commandList, RendererPipelineType pipelineType, BlendMode blendMode);
	void SetShadowPipeline(ID3D12GraphicsCommandList* commandList, ShadowPipelineType pipelineType);

	void SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType pipelineType);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	DXBlendState blendState_;

	std::unique_ptr<DXShaderCompiler> shaderCompiler_;

	std::unique_ptr<DXRootSignature> rootSignature_;

	std::unique_ptr<DXDepthRaster> depthRaster_;

	std::unique_ptr<DXInputLayout> inputLayout_;

	//* postProcess *//

	std::array<ComPtr<ID3D12PipelineState>, postProcessPipelineNum> postProcessPipeline_;

	//* renderer *//

	std::array<std::array<ComPtr<ID3D12PipelineState>, blendModeNum>, rendererPipelineNum> rendererPipeline_;

	//* shadow *//

	std::array<ComPtr<ID3D12PipelineState>, shadowPipelineNum> shadowPipeline_;

	//* compute *//

	std::array<ComPtr<ID3D12PipelineState>, computePipelineNum> computePipeline_;

	//========================================================================*/
	//* functions

	void CreatePostProcessPipeline(ID3D12Device* device,const PostProcessPipelineType& pipelineType);

	void CreateRendererPipeline(ID3D12Device* device, const RendererPipelineType& pipelineType, const BlendMode& blendMode);

	void CreateShadowPipeline(ID3D12Device* device, const ShadowPipelineType& pipelineType);

	void CreateComputePipeline(ID3D12Device* device, const ComputePipelineType& pipelineType);

};