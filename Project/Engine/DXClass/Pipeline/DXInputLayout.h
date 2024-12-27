#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/DXClass/ComPtr.h>
#include <Engine/DXClass/Pipeline/PipelineTypes.h>

// directX
#include <d3d12.h>

//============================================================================*/
//	DXInputLayout class
//============================================================================*/
class DXInputLayout {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DXInputLayout() = default;
	~DXInputLayout() = default;

	void Create(const RendererPipelineType& pipelineType);
	void Create(const ShadowPipelineType& pipelineType);

	//* getter *//

	D3D12_INPUT_LAYOUT_DESC GetDesc() const { return inputLayoutDesc_; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_;

};