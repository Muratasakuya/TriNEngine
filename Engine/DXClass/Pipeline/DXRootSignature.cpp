#include "DXRootSignature.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	DXRootSignature classMethods
//============================================================================*/

void DXRootSignature::Create(const PostProcessPipelineType& pipelineType) {

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	// descriptorRangeの設定
	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
	descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

	// RootParameter作成
	D3D12_ROOT_PARAMETER rootParameters[1]{};

	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
	rootParameters[0].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
	rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

	descriptionRootSignature.pParameters = rootParameters;
	descriptionRootSignature.NumParameters = _countof(rootParameters);

	// Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
	staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

	HRESULT hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	assert(SUCCEEDED(hr));

	hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&postProcessRootSignature_[pipelineType]));
	assert(SUCCEEDED(hr));

}

void DXRootSignature::Create(const RendererPipelineType& pipelineType) {

	HRESULT hr;

	if (pipelineType == Object2D) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[3]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rendererRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == NormalObject3D) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		// texture
		D3D12_DESCRIPTOR_RANGE textureDescriptorRange[1] = {};
		textureDescriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		textureDescriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		textureDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		textureDescriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[6]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTexture
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;             // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                       // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = textureDescriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(textureDescriptorRange); // Tableで利用する数
		// gPunctual
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド
		// gShadowLight
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[5].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rendererRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == TargetShadowObject3D) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		// texture
		D3D12_DESCRIPTOR_RANGE textureDescriptorRange[1] = {};
		textureDescriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		textureDescriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		textureDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		textureDescriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算
		// shadow
		D3D12_DESCRIPTOR_RANGE shadowDescriptorRange[1] = {};
		shadowDescriptorRange[0].BaseShaderRegister = 1;                      // 0から始まる t1
		shadowDescriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		shadowDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		shadowDescriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[7]{};

		// gMaterial
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;    // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTransformationMatrix
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド
		// gTexture
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;             // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                       // PixelShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = textureDescriptorRange;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(textureDescriptorRange); // Tableで利用する数
		// gPunctual
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[3].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gCamera
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;   // PixelShaderで使う
		rootParameters[4].Descriptor.ShaderRegister = 2;                      // レジスタ番号2とバインド
		// gShadowLight
		rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[5].Descriptor.ShaderRegister = 1;                      // レジスタ番号1とバインド
		// gShadow
		rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;             // DescriptorTableを使う
		rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                       // PixelShaderで使う
		rootParameters[6].DescriptorTable.pDescriptorRanges = shadowDescriptorRange;             // Tableの中身の配列を指定
		rootParameters[6].DescriptorTable.NumDescriptorRanges = _countof(shadowDescriptorRange); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rendererRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == NormalParticle) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRange[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// descriptorRangeForInstancingの設定
		D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
		descriptorRangeForInstancing[0].BaseShaderRegister = 0;                      // 0から始まる t0
		descriptorRangeForInstancing[0].NumDescriptors = 1;                          // 数は1つ
		descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRVを使う
		descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                       // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[3]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;      // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;  // VertexShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                      // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;      // DescriptorTableを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;                // PixelShaderで使う
		rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;             // Tableの中身の配列を指定
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange); // Tableで利用する数

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;                   // DescriptorTableを使う
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;                            // VertexShaderで使う
		rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;             // Tableの中身の配列を指定
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing); // Tableで利用する数

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// Samplerの設定
		D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
		staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;         // バイリニアフィルタ
		staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;       // 0~1の範囲外をリピート
		staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;                       // ありったけのMipMapを使う
		staticSamplers[0].ShaderRegister = 0;                               // レジスタ番号0を使う
		staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		descriptionRootSignature.pStaticSamplers = staticSamplers;
		descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rendererRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	} else if (pipelineType == PrimitiveLine) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;    // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; // PixelShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                    // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;     // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 0;                     // レジスタ番号0とバインド

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rendererRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	}

}

void DXRootSignature::Create(const ShadowPipelineType& pipelineType) {

	HRESULT hr;

	if (pipelineType == ShadowDepth) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[2]{};

		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;     // CBVを使う
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[0].Descriptor.ShaderRegister = 0;                     // レジスタ番号0とバインド

		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;     // CBVを使う
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; // VertexShaderで使う
		rootParameters[1].Descriptor.ShaderRegister = 1;                     // レジスタ番号1とバインド


		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&shadowRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	}

}

void DXRootSignature::Create(const ComputePipelineType& pipelineType) {

	HRESULT hr;

	if (pipelineType == SkinningCS) {

		D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

		descriptionRootSignature.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

		// descriptorRangeの設定
		// gMatrixPalette t0
		D3D12_DESCRIPTOR_RANGE matrixPaletteDescRange[1] = {};
		matrixPaletteDescRange[0].BaseShaderRegister = 0;                      // t0
		matrixPaletteDescRange[0].NumDescriptors = 1;                          // 1つ
		matrixPaletteDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		matrixPaletteDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                              // Offsetを自動計算
		// gInputVertices t1
		D3D12_DESCRIPTOR_RANGE inputVerticesDescRange[1] = {};
		inputVerticesDescRange[0].BaseShaderRegister = 1;                      // t1
		inputVerticesDescRange[0].NumDescriptors = 1;                          // 1つ
		inputVerticesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		inputVerticesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                              // Offsetを自動計算
		// gInfluences t2
		D3D12_DESCRIPTOR_RANGE influencesDescRange[1] = {};
		influencesDescRange[0].BaseShaderRegister = 2;                      // t2
		influencesDescRange[0].NumDescriptors = 1;                          // 1つ
		influencesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; // SRV
		influencesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                           // Offsetを自動計算
		// gOutputVertices u0
		D3D12_DESCRIPTOR_RANGE outputVerticesDescRange[1] = {};
		outputVerticesDescRange[0].BaseShaderRegister = 0;                      // u0
		outputVerticesDescRange[0].NumDescriptors = 1;                          // 1つ
		outputVerticesDescRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV; // UAV
		outputVerticesDescRange[0].OffsetInDescriptorsFromTableStart =
			D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;                               // Offsetを自動計算

		// RootParameter作成
		D3D12_ROOT_PARAMETER rootParameters[5]{};

		// gMatrixPalette t0
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[0].DescriptorTable.pDescriptorRanges = matrixPaletteDescRange;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = _countof(matrixPaletteDescRange);
		// gInputVertices t1
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[1].DescriptorTable.pDescriptorRanges = inputVerticesDescRange;
		rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(inputVerticesDescRange);
		// gInfluences t2
		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[2].DescriptorTable.pDescriptorRanges = influencesDescRange;
		rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(influencesDescRange);
		// gOutputVertices u0
		rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[3].DescriptorTable.pDescriptorRanges = outputVerticesDescRange;
		rootParameters[3].DescriptorTable.NumDescriptorRanges = _countof(outputVerticesDescRange);
		// gSkinningInformation b0
		rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
		rootParameters[4].Descriptor.ShaderRegister = 0;

		descriptionRootSignature.pParameters = rootParameters;
		descriptionRootSignature.NumParameters = _countof(rootParameters);

		// バイナリをもとに生成
		hr = D3D12SerializeRootSignature(&descriptionRootSignature,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
		if (FAILED(hr)) {

			assert(false);
		}

		hr = GraphicsEngine::GetDevice()->Get()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
			signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&computeRootSignature_[pipelineType]));
		assert(SUCCEEDED(hr));
	}

}