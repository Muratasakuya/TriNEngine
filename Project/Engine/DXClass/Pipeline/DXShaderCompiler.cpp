#include "DXShaderCompiler.h"

//============================================================================*/
//	include
//============================================================================*/

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dxcompiler.lib")

//============================================================================*/
//	DXShaderCompiler classMethods
//============================================================================*/

IDxcBlob* DXShaderCompiler::CompileShader(
	const std::wstring& filePath, const wchar_t* profile,
	IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {

	// hlslファイルを読み込む
	IDxcBlobEncoding* shaderSouce = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSouce);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSouce->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSouce->GetBufferSize();
	// UTF8の文字コードであることを通知
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
		IID_PPV_ARGS(&shaderResult));
	// コンパイルエラーではなくdxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		assert(false);
	}

	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// もう使わないリソースを解放
	shaderSouce->Release();
	shaderResult->Release();
	shaderError.Reset();

	return shaderBlob;
}

void DXShaderCompiler::Init() {

	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;
	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));

	includeHandler_ = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}

void DXShaderCompiler::Compile(const PostProcessPipelineType& pipelineType) {

	const wchar_t* vsShaderPath = L"./Resources/Engine/Shaders/Fullscreen.VS.hlsl";

	const wchar_t* psShaderPath = nullptr;
	switch (pipelineType) {
	case PostProcessPipelineType::CopyTexture:
		psShaderPath = L"./Resources/Engine/Shaders/CopyImage.PS.hlsl";
		break;
	case PostProcessPipelineType::Vignette:
		psShaderPath = L"./Resources/Engine/Shaders/Vignette.PS.hlsl";
		break;
	case PostProcessPipelineType::RadialBlur:
		psShaderPath = L"./Resources/Engine/Shaders/RadialBlur.PS.hlsl";
		break;
	default:
		assert(false && "Unsupported pipeline type");
		return;
	}

	postProcessVSBlob_[pipelineType] = CompileShader(vsShaderPath, L"vs_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(postProcessVSBlob_[pipelineType] != nullptr);

	postProcessPSBlob_[pipelineType] = CompileShader(psShaderPath, L"ps_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(postProcessPSBlob_[pipelineType] != nullptr);

}

void DXShaderCompiler::Compile(const RendererPipelineType& pipelineType) {

	const wchar_t* vsShaderPath = nullptr;
	const wchar_t* psShaderPath = nullptr;
	switch (pipelineType) {
	case RendererPipelineType::Object2D:
		vsShaderPath = L"./Resources/Engine/Shaders/Object2D.VS.hlsl";
		psShaderPath = L"./Resources/Engine/Shaders/Object2D.PS.hlsl";
		break;
	case RendererPipelineType::NormalObject3D:
		vsShaderPath = L"./Resources/Engine/Shaders/Object3D.VS.hlsl";
		psShaderPath = L"./Resources/Engine/Shaders/NormalObject3D.PS.hlsl";
		break;
	case RendererPipelineType::TargetShadowObject3D:
		vsShaderPath = L"./Resources/Engine/Shaders/Object3D.VS.hlsl";
		psShaderPath = L"./Resources/Engine/Shaders/TargetShadowObject3D.PS.hlsl";
		break;
	case RendererPipelineType::PrimitiveLine:
		vsShaderPath = L"./Resources/Engine/Shaders/PrimitiveLine.VS.hlsl";
		psShaderPath = L"./Resources/Engine/Shaders/PrimitiveLine.PS.hlsl";
		break;
	case RendererPipelineType::NormalParticle:
		vsShaderPath = L"./Resources/Engine/Shaders/Particle.VS.hlsl";
		psShaderPath = L"./Resources/Engine/Shaders/Particle.PS.hlsl";
		break;
	default:
		assert(false && "Unsupported pipeline type");
		return;
	}

	rendererVSBlob_[pipelineType] = CompileShader(vsShaderPath, L"vs_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(rendererVSBlob_[pipelineType] != nullptr);

	rendererPSBlob_[pipelineType] = CompileShader(psShaderPath, L"ps_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(rendererPSBlob_[pipelineType] != nullptr);

}

void DXShaderCompiler::Compile(const ShadowPipelineType& pipelineType) {

	const wchar_t* vsShaderPath = nullptr;
	switch (pipelineType) {
	case ShadowPipelineType::ShadowDepth:
		vsShaderPath = L"./Resources/Engine/Shaders/ShadowDepth.VS.hlsl";
		break;
	default:
		assert(false && "Unsupported pipeline type");
		return;
	}

	shadowVSBlob_[pipelineType] = CompileShader(vsShaderPath, L"vs_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(shadowVSBlob_[pipelineType] != nullptr);

}

void DXShaderCompiler::Compile(const ComputePipelineType& pipelineType) {

	const wchar_t* csShaderPath = nullptr;
	switch (pipelineType) {
	case ComputePipelineType::SkinningCS:
		csShaderPath = L"./Resources/Engine/Shaders/Skinning.CS.hlsl";
		break;
	default:
		assert(false && "Unsupported pipeline type");
		return;
	}

	computeBlob_[pipelineType] = CompileShader(csShaderPath, L"cs_6_0",
		dxcUtils_.Get(), dxcCompiler_.Get(), includeHandler_.Get());
	assert(computeBlob_[pipelineType] != nullptr);

}