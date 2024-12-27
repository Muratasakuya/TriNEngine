#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Engine/Utility/AssetStructure.h>
#include <Lib/MathUtils/Vector2.h>
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Vector4.h>

// c++
#include <utility>
#include <cstdint>

//============================================================================*/
//	bufferSize
//============================================================================*/
struct SpriteVertexData {

	Vector2 pos;
	Vector2 texcoord;
};

//============================================================================*/
//	PrimitiveVertexBuffer class
//============================================================================*/
class PrimitiveVertexBuffer :
	public DXConstBuffer<Vector4> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PrimitiveVertexBuffer() = default;
	~PrimitiveVertexBuffer() = default;

	//========================================================================*/
	//* variables

	std::vector<Vector4> pos;

};

//============================================================================*/
//	VertexBuffer class
//============================================================================*/
template<typename T>
class VertexBuffer :
	public DXConstBuffer<T> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	VertexBuffer() = default;
	~VertexBuffer() = default;

	void Update();

	//========================================================================*/
	//* variables

	std::vector<T> data;

};

//============================================================================*/
//	VertexBuffer class
//============================================================================*/
class IndexBuffer :
	public DXConstBuffer<uint32_t> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	IndexBuffer() = default;
	~IndexBuffer() = default;

	void Update();

	//========================================================================*/
	//* variables

	std::vector<uint32_t> data;

};

//============================================================================*/
//	InputVertexBuffer class
//============================================================================*/
class InputVertexBuffer :
	public DXConstBuffer<ModelVertexData> {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	struct InputVertex {

		std::vector<ModelVertexData> data;
		uint32_t srvIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> srvHandle;
	};

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	InputVertexBuffer() = default;
	~InputVertexBuffer() = default;

	void Init(UINT vertexNum, ID3D12Resource* vertexResource);

	//* getter *//

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return inputVertex.srvHandle.second; }

	//========================================================================*/
	//* variables

	InputVertex inputVertex;

};

//============================================================================*/
//	OutputVertexBuffer class
//============================================================================*/
class OutputVertexBuffer :
	public DXConstBuffer<ModelVertexData> {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	struct OutputVertex {

		std::vector<ModelVertexData> data;
		uint32_t uavIndex;
		std::pair<D3D12_CPU_DESCRIPTOR_HANDLE, D3D12_GPU_DESCRIPTOR_HANDLE> uavHandle;
	};

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	OutputVertexBuffer() = default;
	~OutputVertexBuffer() = default;

	void Init(UINT vertexNum);

	//* getter *//

	D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle() const { return outputVertex.uavHandle.second; }

	//========================================================================*/
	//* variables

	OutputVertex outputVertex;

};

//============================================================================*/
//	SkinningInfoData class
//============================================================================*/
class SkinningInfoData :
	public DXConstBuffer<uint32_t> {
public:
	//===================================================================*/
	//							public Functions
	//===================================================================*/

	SkinningInfoData() = default;
	~SkinningInfoData() = default;

	void Init(UINT vertexNum);

public:
	//===================================================================*/
	//							public Variables
	//===================================================================*/

	uint32_t numVertices;

};