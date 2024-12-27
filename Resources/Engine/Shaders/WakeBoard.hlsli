
struct VertexShaderOutput {
    
	float4 position : SV_POSITION;
	float2 texcoordBase : TEXCOORD0;
	float2 texcoordBlue : TEXCOORD1;
	float2 texcoordWhite : TEXCOORD2;
	float3 normal : NORMAL0;
	float3 worldPosition : POSITION0;
};