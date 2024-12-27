#include "InputAssembler.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Base/GraphicsEngine.h>

//============================================================================*/
//	InputAssembler classMethods
//============================================================================*/

void InputAssembler::Init(const MeshModelData& meshData) {

	//===================================================================*/
	//* vertex

	vertices.emplace_back();

	vertices.back().CreateVertexBuffer(GraphicsEngine::GetDevice()->Get(), static_cast<UINT>(meshData.vertices.size()));
	vertices.back().data.resize(static_cast<UINT>(meshData.vertices.size()));

	std::copy(meshData.vertices.begin(), meshData.vertices.end(), vertices.back().data.begin());
	vertices.back().Update();

	//===================================================================*/
	//* index

	indices.emplace_back();

	indices.back().CreateIndexBuffer(GraphicsEngine::GetDevice()->Get(), static_cast<UINT>(meshData.indices.size()));
	indices.back().data.resize(static_cast<UINT>(meshData.indices.size()));

	std::copy(meshData.indices.begin(), meshData.indices.end(), indices.back().data.begin());
	indices.back().Update();

}

void InputAssembler::SetBuffer(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex) {

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vertices[meshIndex].GetVertexBuffer());
	commandList->IASetIndexBuffer(&indices[meshIndex].GetIndexBuffer());
}

void InputAssembler::DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex) {

	commandList->DrawIndexedInstanced(static_cast<UINT>(indices[meshIndex].data.size()), 1, 0, 0, 0);
}

void InputAssembler::DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t instanceNum, uint32_t meshIndex) {

	commandList->DrawIndexedInstanced(static_cast<UINT>(indices[meshIndex].data.size()), instanceNum, 0, 0, 0);
}