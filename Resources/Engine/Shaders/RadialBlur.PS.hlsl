#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {

	float4 color : SV_TARGET0;
};

// ConstBufferで渡すべき
// 中心点、ここを基準に放射状にブラーがかかる
static const float2 kCenter = float2(0.5f, 0.5f);
// サンプリング数、多いほど滑らかだけど重い
static const int kNumSamples = 10;
// ぼかしの幅、大きいほど大きい
static const float kBlurWidth = 0.01f;

PixelShaderOutput main(VertexShaderOutput input) {

	PixelShaderOutput output;
	output.color = gTexture.Sample(gSampler, input.texcoord);
	
	// 中心から現在のuvに対しての方向を計算
	// 遠ければ遠いほど遠くをサンプリングする、なので正規化しない
	float2 direction = input.texcoord - kCenter;
	float3 outputColor = float3(0.0f, 0.0f, 0.0f);
	
	for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex) {
		
		// 現在のuvから先程計算した方向にサンプリング点を進めながらサンプリングしていく
		float2 texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);
		texcoord = clamp(texcoord, float2(0.0f, 0.0f), float2(0.9f, 0.9f));
		outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
	}
	
	// 平滑化する
	outputColor.rgb *= rcp(float(kNumSamples));
	output.color.rgb = outputColor;
	output.color.a = 1.0f;
	
	return output;
}