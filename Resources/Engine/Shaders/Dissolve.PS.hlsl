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
	
	// mask�̒l��臒l�ȉ��̏ꍇ��discard
	if (mask <= gMaterial.threshold) {
		
		discard;
	}
	
	// Edge���ۂ����Z�o
	float edge = 1.0f - smoothstep(gMaterial.threshold, gMaterial.threshold + gMaterial.edgeSize, mask);
	output.color = gTexture.Sample(gSampler, input.texcoord);
	// Edge���ۂ��قǎw�肵���F�����Z
	output.color.rgb = lerp(output.color.rgb, gMaterial.edgeColor, edge);

	return output;
}