#include "DXInputLayout.h"

//============================================================================*/
//	DXInputLayout classMethods
//============================================================================*/

void DXInputLayout::Create(const RendererPipelineType& pipelineType) {

	inputLayoutDesc_ = D3D12_INPUT_LAYOUT_DESC();

	if (pipelineType == PrimitiveLine) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[1]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_ = {};
		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	} else if (pipelineType == Object2D) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[2]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_ = {};
		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	} else if (pipelineType == NormalObject3D || pipelineType == TargetShadowObject3D) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_ = {};
		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	}  else if (pipelineType == NormalParticle) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "COLOR";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_ = {};
		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	}

}

void DXInputLayout::Create(const ShadowPipelineType& pipelineType) {

	inputLayoutDesc_ = D3D12_INPUT_LAYOUT_DESC();

	if (pipelineType == ShadowDepth) {

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[3]{};

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc_ = {};
		inputLayoutDesc_.pInputElementDescs = inputElementDescs;
		inputLayoutDesc_.NumElements = _countof(inputElementDescs);
	}

}