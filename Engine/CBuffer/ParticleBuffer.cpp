#include "ParticleBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	ParticleBuffer classMethods
//============================================================================*/

void ParticleBuffer::Init(UINT instanceCount) {

	rootParameterIndex = 0;

	DXConstBuffer::CreateStructuredBuffer(GraphicsEngine::GetDevice()->Get(), instanceCount);

}

void ParticleBuffer::Update() {

	DXConstBuffer::TransferVectorData(properties);
}