#include "Object3D.hlsli"

/*===========================================================
              TargetShadowObject3D PS Shader
===========================================================*/

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

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<PunctualLight> gPunctual : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float4> gTexture : register(t0);
Texture2D<float3> gShadowTexture : register(t1);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input) {
   
	PixelShaderOutput output;

	float4 transformUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
	float4 textureColor = gTexture.Sample(gSampler, transformUV.xy);
	
	// w除算で正規化スクリーン座標系に変換する
	float2 shadowMapUV = input.positionInLVP.xy / input.positionInLVP.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	
	// ライトビュースクリーン空間でのZ値を計算する
	float zInLVp = input.positionInLVP.z / input.positionInLVP.w;
	
	float3 shadowMap = 1.0f;
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f) {
		
		// 計算したUV座標を使ってシャドウマップからの深度値をサンプリング
		float zInShadowMap = gShadowTexture.Sample(gSampler, shadowMapUV).r;
		if (zInLVp > zInShadowMap) {
			
			// 隠蔽されている
			textureColor.xyz *= 0.25f;
		}
	}
	
	output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	output.color.rgb *= shadowMap;
	output.color.a = gMaterial.color.a * textureColor.a;

	return output;
}