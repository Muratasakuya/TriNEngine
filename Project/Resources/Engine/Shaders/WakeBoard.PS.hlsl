#include "WakeBoard.hlsli"

/// マテリアル構造体
struct Material {
    
	float4 color; // オブジェクトの色
	int enableLighting; // ライティングの有無
	int enableHalfLambert; // ハ－フランバートの有無
	int enablePhongReflection; // フォン反射の有無
	int enableBlinnPhongReflection; // ブリン-フォン反射の有無
	float shininess; // 輝度
	float3 specularColor; // 鏡面反射色
	float4x4 uvTransform; // UVTransform
};

/// 平行光源
struct DirectionalLight {

	float4 color; // 色
	float3 direction; // 向き
	float intensity; // 輝度 強さ
};

/// ポイントライト
struct PointLight {
	
	float4 color; // 色
	float3 pos; // 座標
	float intensity; // 輝度 強さ
	float radius; // 半径,サイズ
	float decay; // 減衰率
};

/// スポットライト
struct SpotLight {

	float4 color; // 色
	float3 pos; // 座標
	float intensity; // 輝度 強さ
	float3 direction; // 向き
	float distance; // 距離
	float decay; // 減衰率
	float cosAngle; // 回転角
	float cosFalloffStart; // 初期回転角
};

// 全てのライト
struct PunctualLight {
	
	DirectionalLight directionalLight; // 平行光源
	PointLight pointLight; // ポイントライト 本当はpointと書きたいがpointは既存と競合して使えない。しかたなく他も合わせる
	SpotLight spotLight; // スポットライト
};

// カメラ
struct Camera {

	float3 worldPosition;
};

struct PixelShaderOutput {
    
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);             // レジスタ 0
ConstantBuffer<PunctualLight> gPunctual : register(b1);        // レジスタ 1
ConstantBuffer<Camera> gCamera : register(b2);                 // レジスタ 2

Texture2D<float4> gBaseTexture : register(t0);  // BaseWhite
Texture2D<float4> gBlueTexture : register(t1);  // BlueWave
Texture2D<float4> gWhiteTexture : register(t2); // WhiteWave
SamplerState gSampler : register(s0);           // Sampler

PixelShaderOutput main(VertexShaderOutput input) {
	
	PixelShaderOutput output;

	// TextureColor * MaterialColor
	float4 baseColor = gBaseTexture.Sample(gSampler, input.texcoordBase) * gMaterial.color;
	float4 blueWaveColor1 = gBlueTexture.Sample(gSampler, input.texcoordBlue) * gMaterial.color;
	float4 blueWaveColor2 = gWhiteTexture.Sample(gSampler, input.texcoordWhite) * gMaterial.color;

	// 減算
	float4 combinedBlueWaves = blueWaveColor1 - blueWaveColor2;

	// 色の合成
	float3 finalColor = baseColor.rgb * (1.0f - combinedBlueWaves.a) + combinedBlueWaves.rgb * combinedBlueWaves.a;

	if (gMaterial.enableLighting == 1) {

		// Half Lambert
		float NdotL = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
		float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);

		float3 lightColor = gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity;
		finalColor *= lightColor * cos;
	}

	output.color.rgb = finalColor;
	output.color.a = max(baseColor.a, combinedBlueWaves.a);

	if (output.color.a <= 0.5f) {
		discard;
	}

	return output;
}