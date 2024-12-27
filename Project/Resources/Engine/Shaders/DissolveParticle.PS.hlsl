#include "Particle.hlsli"

/*===========================================================
                DissolveParticle PS Shader
===========================================================*/

struct DissolveMaterial {

	float threshold;
	float edgeSize;
	float3 edgeColor;
};

struct PixelShaderOutput {
    
	float4 color : SV_TARGET0;
};

ConstantBuffer<DissolveMaterial> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t0);
Texture2D<float> gMaskTexture : register(t1);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {

	float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
	float maskValue = gMaskTexture.Sample(gSampler, input.texcoord).r;

	PixelShaderOutput output;

	// texture‚Ìƒ¿’l‚ª0.5fˆÈ‰º‚ÌŽž‚ÉPixel‚ðŠü‹p
	if (textureColor.a <= 0.5f) {

		discard;
	}

	float dissolveFactor =
	smoothstep(gMaterial.threshold - gMaterial.edgeSize, gMaterial.threshold + gMaterial.edgeSize, maskValue);

	output.color.rgb = lerp(textureColor.rgb, gMaterial.edgeColor, 1.0f - dissolveFactor);
	output.color.a = textureColor.a * dissolveFactor * input.color.a;

	return output;
}