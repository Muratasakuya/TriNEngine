#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

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
	gTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(rcp(width), rcp(height));

	// 縦横それぞれの畳み込み結果を格納する
	float2 difference = float2(0.0f, 0.0f);
	// 色を輝度に変換して、畳み込みを行っていく。微分Filter用のKernelになっているので、やること自体は今までの畳み込みと同じ
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			
			float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
			float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			float luminance = Luminance(fetchColor);
			
			difference.x += luminance * kPrewittHorizontalKernel[x][y];
			difference.y += luminance * kPrewittVerticalKernel[x][y];
		}
	}
	
	// 変化の長さをウェイトとして合成。ウェイトの決定方法も色々と考えられる。例えばdifference.xだけ使えば横方向のエッジが検出される
	float weight = length(difference);
	// とりあえず6倍、ConstBufferで調整パラメータとして送るのがベスト
	weight = saturate(weight * 6.0f);
	
	output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler,input.texcoord).rgb;
	output.color.a = 1.0f;

	return output;
}