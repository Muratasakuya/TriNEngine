#include "Object2d.hlsli"

struct Obj2DMaterial {
	
	float4 color;
	float4x4 uvTransform;
};

ConstantBuffer<Obj2DMaterial> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	
	float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
    
	PixelShaderOutput output;
	output.color = gMaterial.color * textureColor;
	output.color = pow(output.color, 2.2f);
    
	return output;
}