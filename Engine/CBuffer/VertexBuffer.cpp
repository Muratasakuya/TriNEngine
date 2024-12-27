#include "VertexBuffer.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	VertexBuffer classMethod
//============================================================================*/

template class VertexBuffer<ModelVertexData>;
template class VertexBuffer<SpriteVertexData>;

template<>
void VertexBuffer<ModelVertexData>::Update() {

	DXConstBuffer::TransferVectorData(data);

}

template<>
void VertexBuffer<SpriteVertexData>::Update() {

	DXConstBuffer::TransferVectorData(data);

}

//============================================================================*/
//	IndexBuffer classMethod
//============================================================================*/

void IndexBuffer::Update() {

	DXConstBuffer::TransferVectorData(data);

}

//============================================================================*/
//	SkinningInfoData classMethod
//============================================================================*/

void SkinningInfoData::Init(UINT vertexNum) {

	numVertices = vertexNum;

	DXConstBuffer::CreateConstBuffer(GraphicsEngine::GetDevice()->Get());
	DXConstBuffer::TransferData(numVertices);

}

//============================================================================*/
//	InputVertexBuffer classMethod
//============================================================================*/

void InputVertexBuffer::Init(UINT vertexNum, ID3D12Resource* vertexResource) {

	// SRV確保
	inputVertex.srvIndex = GraphicsEngine::SRV()->Allocate();
	inputVertex.srvHandle.first = GraphicsEngine::SRV()->GetCPUHandle(inputVertex.srvIndex);
	inputVertex.srvHandle.second = GraphicsEngine::SRV()->GetGPUHandle(inputVertex.srvIndex);
	// SRV生成
	GraphicsEngine::SRV()->CreateSRVForStructureBuffer(
		inputVertex.srvIndex, vertexResource, vertexNum, static_cast<UINT>(sizeof(ModelVertexData)));
}

//============================================================================*/
//	OutputVertexBuffer classMethod
//============================================================================*/

void OutputVertexBuffer::Init(UINT vertexNum) {

	DXConstBuffer::CreateUavVertexBuffer(GraphicsEngine::GetDevice()->Get(), vertexNum);

	// UAV確保
	outputVertex.uavIndex = GraphicsEngine::SRV()->Allocate();
	outputVertex.uavHandle.first = GraphicsEngine::SRV()->GetCPUHandle(outputVertex.uavIndex);
	outputVertex.uavHandle.second = GraphicsEngine::SRV()->GetGPUHandle(outputVertex.uavIndex);
	// UAV生成
	GraphicsEngine::SRV()->CreateUAVForStructureBuffer(
		outputVertex.uavIndex, this->GetResource(), vertexNum, static_cast<UINT>(sizeof(ModelVertexData)));

}