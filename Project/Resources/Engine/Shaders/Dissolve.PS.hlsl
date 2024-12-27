#include "Fullscreen.hlsli"

struct Material {
	
	float threshold;
	float edgeSize;
	float3 edgeColor;
};

struct PixelShaderOutput {

	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input) {

	PixelShaderOutput output;
	
	float mask = gMaskTexture.Sample(gSampler, input.texcoord);
	
	// maskの値が閾値以下の場合はdiscard
	if (mask <= gMaterial.threshold) {
		
		discard;
	}
	
	// Edgeっぽさを算出
	float edge = 1.0f - smoothstep(gMaterial.threshold, gMaterial.threshold + gMaterial.edgeSize, mask);
	output.color = gTexture.Sample(gSampler, input.texcoord);
	// Edgeっぽいほど指定した色を加算
	output.color.rgb = lerp(output.color.rgb, gMaterial.edgeColor, edge);

	return output;
}