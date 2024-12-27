
struct Vertex {
	
	float4 position;
	float2 texcoord;
	float3 normal;
};

struct Well {
	
	float4x4 skeletonSpaceMatrix;
	float4x4 skeletonSpaceInverseTransposeMatrix;
};

struct VertexInfluence {
	
	float4 weight;
	int4 index;
};

struct SkinningInformation {
	
	uint numVertices;
};

StructuredBuffer<Well> gMatrixPalette : register(t0);
StructuredBuffer<Vertex> gInputVertices : register(t1);
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// Skinning�v�Z��̒��_�f�[�^
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID) {
	
	// ���_�C���f�b�N�X
	uint vertexIndex = DTid.x;
	// �͈͊O��� (�o�b�t�@�I�[�o�[����)
	if (vertexIndex < gSkinningInformation.numVertices) {
		
		Vertex input = gInputVertices[vertexIndex];
		VertexInfluence influence = gInfluences[vertexIndex];
		
		// Skinning��̒��_���v�Z
		Vertex skinned;
		skinned.texcoord = input.texcoord;
		
		// �ʒu�̕ϊ�
		skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
		skinned.position.w = 1.0f;
		// �@���̕ϊ�
		skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
		// ���K�����Ė߂�
		skinned.normal = normalize(skinned.normal);
		
		// Skinning��̒��_�f�[�^���i�[
		gOutputVertices[vertexIndex] = skinned;
	}
	
}