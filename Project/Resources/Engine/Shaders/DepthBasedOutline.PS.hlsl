#include "Fullscreen.hlsli"

/// マテリアル構造体
struct Material {
	
	float4x4 projectionInverse;
};

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);

// Render
Texture2D<float4> gRenderTexture : register(t0);
SamplerState gSamplerLinear : register(s0);
// Depth
Texture2D<float> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

static const float2 kIndex3x3[3][3] = {
	{ { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
	{ { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

// 横
static const float kPrewittHorizontalKernel[3][3] = {
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};
// 縦
static const float kPrewittVerticalKernel[3][3] = {
	{ -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }
};

// RGB -> 輝度
float Luminance(float3 v) {
	return dot(v, float3(0.2125f, 0.7145f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input) {

	PixelShaderOutput output;

    // uvStepSizeの算出
	int width, height;
	gRenderTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(1.0f / width, 1.0f / height);

    // 縦横それぞれの畳み込み結果を格納する
	float2 difference = float2(0.0f, 0.0f);
    // 色を輝度に変換して、畳み込みを行っていく
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {

			float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
			float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord).r;
            // NDC -> view。P^{-1}においてxとyはzwに影響を与えない
			float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            // 同次座標系からデカルト座標系へ変換
			float viewZ = viewSpace.z * rcp(viewSpace.w);
			difference.x += viewZ * kPrewittHorizontalKernel[x][y];
			difference.y += viewZ * kPrewittVerticalKernel[x][y];
		}
	}

    // 変化の長さをウェイトとして合成
	float weight = length(difference);
	weight = saturate(weight);

	output.color.rgb = (1.0f - weight) * gRenderTexture.Sample(gSamplerLinear, input.texcoord).rgb;
	output.color.a = 1.0f;

	return output;
}