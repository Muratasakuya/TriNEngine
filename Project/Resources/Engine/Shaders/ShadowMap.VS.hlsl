
struct ShadowMap {

	float4x4 lightViewProjection;
};

struct VSInput {
	
	float3 position : POSITION;
};

struct VSOutput {
	
	float4 position : SV_POSITION;
};

ConstantBuffer<ShadowMap> gShadowMap : register(b0);

VSOutput main(VSInput input) {
	
	VSOutput output;
	
	output.position = mul(float4(input.position, 1.0f), gShadowMap.lightViewProjection);
	
	return output;
}