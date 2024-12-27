#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>
#include <dxcapi.h>

// c++
#include <array>
#include <string>
#include <cassert>

//============================================================================*/
//	DXShaderCompiler class
//============================================================================*/
class DXShaderCompiler {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXShaderCompiler() = default;
	~DXShaderCompiler() = default;

	void Init();

	void Compile(const PostProcessPipelineType& pipelineType);
	void Compile(const RendererPipelineType& pipelineType);
	void Compile(const ShadowPipelineType& pipelineType);
	void Compile(const ComputePipelineType& pipelineType);

	//* getter *//

	IDxcBlob* GetPostProcessVSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessVSBlob_[pipelineType].Get(); }
	IDxcBlob* GetPostProcessPSBlob(const PostProcessPipelineType& pipelineType) const { return postProcessPSBlob_[pipelineType].Get(); }

	IDxcBlob* GetRendererVSBlob(const RendererPipelineType& pipelineType) const { return rendererVSBlob_[pipelineType].Get(); }
	IDxcBlob* GetRendererPSBlob(const RendererPipelineType& pipelineType) const { return rendererPSBlob_[pipelineType].Get(); }

	IDxcBlob* GetShadowVSBlob(const ShadowPipelineType& pipelineType) const { return shadowVSBlob_[pipelineType].Get(); }

	IDxcBlob* GetComputeBlob(const ComputePipelineType& pipelineType) const { return computeBlob_[pipelineType].Get(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	ComPtr<IDxcUtils> dxcUtils_;
	ComPtr<IDxcCompiler3> dxcCompiler_;
	ComPtr<IDxcIncludeHandler> includeHandler_;

	//* postProcess *//

	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessVSBlob_;
	std::array<ComPtr<IDxcBlob>, postProcessPipelineNum> postProcessPSBlob_;

	//* renderer *//

	std::array<ComPtr<IDxcBlob>, rendererPipelineNum> rendererVSBlob_;
	std::array<ComPtr<IDxcBlob>, rendererPipelineNum> rendererPSBlob_;

	//* shadow *//

	std::array<ComPtr<IDxcBlob>, shadowPipelineNum> shadowVSBlob_;

	//* compute *//

	std::array<ComPtr<IDxcBlob>, computePipelineNum> computeBlob_;

	//========================================================================*/
	//* function

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);

};