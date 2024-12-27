#include "PipelineManager.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	PipelineManager classMethods
//============================================================================*/

void PipelineManager::CreatePostProcessPipeline(ID3D12Device* device, const PostProcessPipelineType& pipelineType) {

	// BlendState
	D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(kBlendModeNormal);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetPostProcess(pipelineType);
	graphicsPipelineStateDesc.InputLayout.pInputElementDescs = nullptr;
	graphicsPipelineStateDesc.InputLayout.NumElements = 0;
	graphicsPipelineStateDesc.VS =
	{ shaderCompiler_->GetPostProcessVSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetPostProcessVSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.PS =
	{ shaderCompiler_->GetPostProcessPSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetPostProcessPSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendState;
	graphicsPipelineStateDesc.RasterizerState = depthRaster_->GetRasterizerDesc();
	graphicsPipelineStateDesc.DepthStencilState = depthRaster_->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成
	postProcessPipeline_[pipelineType] = nullptr;
	HRESULT hr = device->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&postProcessPipeline_[pipelineType]));
	assert(SUCCEEDED(hr));

}

void PipelineManager::CreateRendererPipeline(ID3D12Device* device,
	const RendererPipelineType& pipelineType, const BlendMode& blendMode) {

	// BlendState
	D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(blendMode);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetRenderer(pipelineType);
	graphicsPipelineStateDesc.InputLayout = inputLayout_->GetDesc();
	graphicsPipelineStateDesc.VS =
	{ shaderCompiler_->GetRendererVSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetRendererVSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.PS =
	{ shaderCompiler_->GetRendererPSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetRendererPSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendState;
	graphicsPipelineStateDesc.RasterizerState = depthRaster_->GetRasterizerDesc();
	graphicsPipelineStateDesc.DepthStencilState = depthRaster_->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	if (pipelineType == PrimitiveLine) {
		graphicsPipelineStateDesc.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	}
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// 生成
	rendererPipeline_[pipelineType][blendMode] = nullptr;
	HRESULT hr = device->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&rendererPipeline_[pipelineType][blendMode]));
	assert(SUCCEEDED(hr));
}

void PipelineManager::CreateShadowPipeline(ID3D12Device* device, const ShadowPipelineType& pipelineType) {

	// BlendState
	D3D12_RENDER_TARGET_BLEND_DESC blendState = blendState_.Create(kBlendModeNormal);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = rootSignature_->GetShadow(pipelineType);
	graphicsPipelineStateDesc.InputLayout = inputLayout_->GetDesc();
	graphicsPipelineStateDesc.VS =
	{ shaderCompiler_->GetShadowVSBlob(pipelineType)->GetBufferPointer(),
		shaderCompiler_->GetShadowVSBlob(pipelineType)->GetBufferSize() };
	graphicsPipelineStateDesc.PS = { nullptr, 0 };
	graphicsPipelineStateDesc.BlendState.RenderTarget[0] = blendState;
	graphicsPipelineStateDesc.RasterizerState = depthRaster_->GetRasterizerDesc();
	graphicsPipelineStateDesc.DepthStencilState = depthRaster_->GetDepthStencilDesc();
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	graphicsPipelineStateDesc.NumRenderTargets = 0;
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = UINT_MAX;

	// 生成
	shadowPipeline_[pipelineType] = nullptr;
	HRESULT hr = device->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&shadowPipeline_[pipelineType]));
	assert(SUCCEEDED(hr));
}

void PipelineManager::CreateComputePipeline(ID3D12Device* device, const ComputePipelineType& pipelineType) {

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};

	computePipelineStateDesc.CS = {
		.pShaderBytecode = shaderCompiler_->GetComputeBlob(pipelineType)->GetBufferPointer(),
		.BytecodeLength = shaderCompiler_->GetComputeBlob(pipelineType)->GetBufferSize()
	};
	computePipelineStateDesc.pRootSignature = rootSignature_->GetCompute(pipelineType);

	// 生成
	HRESULT hr = device->CreateComputePipelineState(
		&computePipelineStateDesc,
		IID_PPV_ARGS(&computePipeline_[pipelineType]));
	assert(SUCCEEDED(hr));
}

void PipelineManager::Create(ID3D12Device* device) {

	shaderCompiler_ = std::make_unique<DXShaderCompiler>();
	rootSignature_ = std::make_unique<DXRootSignature>();
	depthRaster_ = std::make_unique<DXDepthRaster>();
	inputLayout_ = std::make_unique<DXInputLayout>();

	shaderCompiler_->Init();

	//=====================================================================================================================================*/

	for (const auto& postProcessPipelineType : postProcessPipelineTypes) {

		shaderCompiler_->Compile(postProcessPipelineType);

		rootSignature_->Create(postProcessPipelineType);

		depthRaster_->Create();

		CreatePostProcessPipeline(device, postProcessPipelineType);

	}

	//=====================================================================================================================================*/

	for (const auto& rendererPipelineType : rendererPipelineTypes) {

		shaderCompiler_->Compile(rendererPipelineType);

		for (const auto& blendModeType : blendModeTypes) {

			rootSignature_->Create(rendererPipelineType);

			depthRaster_->Create(rendererPipelineType);

			inputLayout_->Create(rendererPipelineType);

			CreateRendererPipeline(device, rendererPipelineType, blendModeType);
		}
	}

	//=====================================================================================================================================*/

	for (const auto& shadowPipelineType : shadowPipelineTypes) {

		shaderCompiler_->Compile(shadowPipelineType);

		rootSignature_->Create(shadowPipelineType);

		depthRaster_->Create(shadowPipelineType);

		inputLayout_->Create(shadowPipelineType);

		CreateShadowPipeline(device, shadowPipelineType);
	}

	//=====================================================================================================================================*/

	for (const auto& computePipelineType : computePipelineTypes) {

		shaderCompiler_->Compile(computePipelineType);

		rootSignature_->Create(computePipelineType);

		CreateComputePipeline(device, computePipelineType);

	}

}

void PipelineManager::SetPostProcessPipeline(
	ID3D12GraphicsCommandList* commandList, PostProcessPipelineType pipelineType) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetPostProcess(pipelineType));
	commandList->SetPipelineState(postProcessPipeline_[pipelineType].Get());

}

void PipelineManager::SetRendererPipeline(
	ID3D12GraphicsCommandList* commandList, RendererPipelineType pipelineType, BlendMode blendMode) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetRenderer(pipelineType));
	commandList->SetPipelineState(rendererPipeline_[pipelineType][blendMode].Get());
}

void PipelineManager::SetShadowPipeline(ID3D12GraphicsCommandList* commandList, ShadowPipelineType pipelineType) {

	commandList->SetGraphicsRootSignature(rootSignature_->GetShadow(pipelineType));
	commandList->SetPipelineState(shadowPipeline_[pipelineType].Get());
}

void PipelineManager::SetComputePipeline(ID3D12GraphicsCommandList* commandList, ComputePipelineType pipelineType) {

	commandList->SetComputeRootSignature(rootSignature_->GetCompute(pipelineType));
	commandList->SetPipelineState(computePipeline_[pipelineType].Get());
}