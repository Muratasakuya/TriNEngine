
struct LightMatrix {
	
	float4x4 lightViewProjection;
};

struct TransformationMatrix {

	float4x4 World;
	float4x4 WVP;
	float4x4 WorldInverseTranspose;
};

struct VertexShaderInput {
	
	float3 position : POSITION;
	
	// •K—v‚È‚¢‚¯‚Ç‚ß‚ñ‚Ç‚¢‚Ì‚Å“ü‚ê‚é
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexShaderOutput {
	
	float4 position : SV_POSITION;
};

ConstantBuffer<LightMatrix> gLightMatrix : register(b0);
ConstantBuffer<TransformationMatrix> gTransformationMatirx : register(b1);

VertexShaderOutput main(VertexShaderInput input) {
	
	VertexShaderOutput output;
	float4 worldPos = float4(input.position, 1.0f);
	output.position = mul(mul(worldPos, gTransformationMatirx.World), gLightMatrix.lightViewProjection);
	
	return output;
}