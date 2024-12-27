#include "CameraBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	CameraBuffer classMethods
//============================================================================*/

void CameraBuffer::Init() {

	rootParameterIndex = 4;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

void CameraBuffer::Update(const Vector3& translate) {

	DXConstBuffer::TransferData(translate);

}

//============================================================================*/
//	ViewProjectionBuffer classMethods
//============================================================================*/

void ViewProjectionBuffer::Init() {

	rootParameterIndex = 1;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

void ViewProjectionBuffer::Update(const Matrix4x4& viewProjection) {

	DXConstBuffer::TransferData(viewProjection);

}

//============================================================================*/
//	LightViewProjectionBuffer classMethods
//============================================================================*/

void LightViewProjectionBuffer::Init() {

	rootParameterIndex = 0;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());

}

void LightViewProjectionBuffer::Update(const Matrix4x4& viewProjection) {

	DXConstBuffer::TransferData(viewProjection);

}