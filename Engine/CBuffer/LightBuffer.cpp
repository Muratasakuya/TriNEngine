#include "LightBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	LightBuffer classMethods
//============================================================================*/

void LightBuffer::Init() {

	rootParameterIndex = 3;

	light.Init();

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

void LightBuffer::Update() {

	DXConstBuffer::TransferData(light);

}