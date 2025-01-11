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
	
	//========================================================================*/
	//* shadow *//
	
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
			textureColor.xyz *= 0.05f;
		}
	}
	
	//========================================================================*/
	//* Lighting *//
	
	// 今回はBlinnPhongのLight処理のみ行う
	if (gMaterial.enableBlinnPhongReflection == 1) {
		
		// PointLightの入射光
		float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
		// PointLightへの距離
		float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
		// 指数によるコントロール
		float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
		// SpotLightの入射光
		float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
		// SpotLightへの距離
		float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
		// 指数によるコントロール
		float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
		// Camera方向算出
		float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		
		/*-------------------------------------------------------------------------------------------------*/
		/// DirectionalLight

		float3 halfVectorDirectionalLight = normalize(normalize(-gPunctual.directionalLight.direction) + toEye);
		float NDotHDirectionalLight = dot(normalize(input.normal), halfVectorDirectionalLight);
		float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);

		float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
		float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);

		// 拡散反射
		float3 diffuseDirectionalLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;

		// 鏡面反射
		float3 specularDirectionalLight =
			gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
		
		/*-------------------------------------------------------------------------------------------------*/
		/// PointLight

		float3 halfVectorPointLight = normalize(-pointLightDirection + toEye);
		float NDotHPointLight = dot(normalize(input.normal), halfVectorPointLight);
		float specularPowPointLight = pow(saturate(NDotHPointLight), gMaterial.shininess);

		float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
		float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);

		// 拡散反射
		float3 diffusePointLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;

		// 鏡面反射
		float3 specularPointLight =
			gPunctual.pointLight.color.rgb * gPunctual.pointLight.intensity * factorPointLight * specularPowPointLight * gMaterial.specularColor;

		/*-------------------------------------------------------------------------------------------------*/
		/// SpotLight

		float3 halfVectorSpotLight = normalize(-spotLightDirectionOnSurface + toEye);
		float NDotHSpotLight = dot(normalize(input.normal), halfVectorSpotLight);
		float specularPowSpotLight = pow(saturate(NDotHSpotLight), gMaterial.shininess);

		float NdotLSpotLight = dot(normalize(input.normal), -spotLightDirectionOnSurface);
		float cosSpotLight = pow(NdotLSpotLight * 0.5f + 0.5f, 2.0f);

		float cosAngle = dot(spotLightDirectionOnSurface, gPunctual.spotLight.direction);
		float falloffFactor = saturate((cosAngle - gPunctual.spotLight.cosAngle) / (gPunctual.spotLight.cosFalloffStart - gPunctual.spotLight.cosAngle));

		// 拡散反射
		float3 diffuseSpotLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight;

		// 鏡面反射
		float3 specularSpotLight =
			gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight * specularPowSpotLight * gMaterial.specularColor;
		
		output.color.rgb =
			diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
		
	} else {
		
		// 影を落とす前の色
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	}
	
	// 影を合成
	output.color.rgb *= shadowMap;
	
	// α値
	output.color.a = gMaterial.color.a * textureColor.a;

	return output;
}