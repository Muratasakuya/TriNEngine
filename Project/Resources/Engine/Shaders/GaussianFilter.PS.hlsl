#include "Fullscreen.hlsli"

// マルチパスレンダリングで縦、横分けてやるとめっちゃ軽くなるらしい

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// 3x3のIndex
static const float2 kIndex3x3[3][3] = {
	{ { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
	{ { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

// π
static const float PI = 3.14159265f;

// ガウス関数
float gauss(float x, float y, float sigma) {
	float exponent = -(x * x + y * y) / (2.0f * sigma * sigma);
	float denominator = 2.0f * PI * sigma * sigma;
	
	return exp(exponent) / denominator;
}

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
	
	PixelShaderOutput output;

	// uvStepSizeの算出
	int width, height;
	gTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(rcp(width), rcp(height));

	output.color.rgb = float3(0.0f, 0.0f, 0.0f);
	output.color.a = 1.0f;

	// カーネルとウェイトの計算
	float kernel3x3[3][3];
	float weight = 0.0f;
	// 標準偏差 ConstBufferで渡す処理を入れてもいい
	float sigma = 2.0f;

	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			
			// ガウス関数を用いてカーネルの値を計算
			kernel3x3[x][y] = gauss(kIndex3x3[x][y].x, kIndex3x3[x][y].y, sigma);
			weight += kernel3x3[x][y];
		}
	}

	// カーネルを使用して畳み込み処理を行う
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {

			// texcoordの計算
			float2 texcoord = input.texcoord + kIndex3x3[i][j] * uvStepSize;
			// テクスチャのサンプル値取得
			float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			// カーネル値を掛けて色を足す
			output.color.rgb += fetchColor * kernel3x3[i][j];
		}
	}

    // 合計値を正規化
	output.color.rgb *= rcp(weight);

	return output;
}
