#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/AssetStructure.h>
#include <Engine/CBuffer/VertexBuffer.h>

//============================================================================*/
//	InputAssembler class
//============================================================================*/
class InputAssembler {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InputAssembler() = default;
	~InputAssembler() = default;

	void Init(const MeshModelData& meshData);

	void SetBuffer(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex);

	void DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t meshIndex);
	void DrawCall(ID3D12GraphicsCommandList* commandList, uint32_t instanceNum, uint32_t meshIndex);

	//* getter *//

	VertexBuffer<ModelVertexData>& GetVertexData() { return vertices.front(); }

	IndexBuffer& GetIndexData() { return indices.front(); }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	std::vector<VertexBuffer<ModelVertexData>> vertices;

	std::vector<IndexBuffer> indices;

};