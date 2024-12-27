#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// 5x5��Index
static const float2 kIndex5x5[5][5] = {
	{ { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
	{ { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
	{ { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
	{ { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
	{ { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } }
};

// 5x5
static const float kKernel5x5[5][5] = {
	{ 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f },
	{ 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f },
	{ 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f },
	{ 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f },
	{ 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f, 1.0f / 25.0f }
};

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	
	PixelShaderOutput output;

	// uvStepSize�̎Z�o
	int width, height;
	gTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(rcp(width), rcp(height));

	output.color.rgb = float3(0.0f, 0.0f, 0.0f);
	output.color.a = 1.0f;

	// 5x5
	for (int x = 0; x < 5; ++x) {
		for (int y = 0; y < 5; ++y) {
			
			// texcoord�̌v�Z
			float2 texcoord = input.texcoord + kIndex5x5[x][y] * uvStepSize;
			float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			// �J�[�l���l���|���ĐF�𑫂�
			output.color.rgb += fetchColor * kKernel5x5[x][y];
		}
	}

	return output;
}