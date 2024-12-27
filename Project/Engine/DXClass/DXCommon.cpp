#include "DXCommon.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	DXCommon classMethods
//============================================================================*/

void DXCommon::SetViewportAndScissor(uint32_t width, uint32_t height) {

	viewport_ =
		D3D12_VIEWPORT(0.0f, 0.0f, float(width), float(height), 0.0f, 1.0f);
	commandList_->RSSetViewports(1, &viewport_);

	scissorRect_ = D3D12_RECT(0, 0, width, height);
	commandList_->RSSetScissorRects(1, &scissorRect_);

}