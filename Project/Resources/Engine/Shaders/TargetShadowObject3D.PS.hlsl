#include "Object3D.hlsli"

/*===========================================================
              TargetShadowObject3D PS Shader
===========================================================*/

/// �}�e���A���\����
struct Material {
    
	float4 color; // �I�u�W�F�N�g�̐F
	int enableLighting; // ���C�e�B���O�̗L��
	int enableHalfLambert; // �n�|�t�����o�[�g�̗L��
	int enablePhongReflection; // �t�H�����˂̗L��
	int enableBlinnPhongReflection; // �u����-�t�H�����˂̗L��
	float shininess; // �P�x
	float3 specularColor; // ���ʔ��ːF
	float4x4 uvTransform; // UVTransform
};

/// ���s����
struct DirectionalLight {

	float4 color; // �F
	float3 direction; // ����
	float intensity; // �P�x ����
};

/// �|�C���g���C�g
struct PointLight {
	
	float4 color; // �F
	float3 pos; // ���W
	float intensity; // �P�x ����
	float radius; // ���a,�T�C�Y
	float decay; // ������
};

/// �X�|�b�g���C�g
struct SpotLight {

	float4 color; // �F
	float3 pos; // ���W
	float intensity; // �P�x ����
	float3 direction; // ����
	float distance; // ����
	float decay; // ������
	float cosAngle; // ��]�p
	float cosFalloffStart; // ������]�p
};

// �S�Ẵ��C�g
struct PunctualLight {
	
	DirectionalLight directionalLight; // ���s����
	PointLight pointLight; // �|�C���g���C�g �{����point�Ə���������point�͊����Ƌ������Ďg���Ȃ��B�������Ȃ��������킹��
	SpotLight spotLight; // �X�|�b�g���C�g
};

// �J����
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
	
	// w���Z�Ő��K���X�N���[�����W�n�ɕϊ�����
	float2 shadowMapUV = input.positionInLVP.xy / input.positionInLVP.w;
	shadowMapUV *= float2(0.5f, -0.5f);
	shadowMapUV += 0.5f;
	
	// ���C�g�r���[�X�N���[����Ԃł�Z�l���v�Z����
	float zInLVp = input.positionInLVP.z / input.positionInLVP.w;
	
	float3 shadowMap = 1.0f;
	if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f &&
		shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f) {
		
		// �v�Z����UV���W���g���ăV���h�E�}�b�v����̐[�x�l���T���v�����O
		float zInShadowMap = gShadowTexture.Sample(gSampler, shadowMapUV).r;
		if (zInLVp > zInShadowMap) {
			
			// �B������Ă���
			textureColor.xyz *= 0.05f;
		}
	}
	
	//========================================================================*/
	//* Lighting *//
	
	// �����BlinnPhong��Light�����̂ݍs��
	if (gMaterial.enableBlinnPhongReflection == 1) {
		
		// PointLight�̓��ˌ�
		float3 pointLightDirection = normalize(input.worldPosition - gPunctual.pointLight.pos);
		// PointLight�ւ̋���
		float distancePointLight = length(gPunctual.pointLight.pos - input.worldPosition);
		// �w���ɂ��R���g���[��
		float factorPointLight = pow(saturate(-distancePointLight / gPunctual.pointLight.radius + 1.0f), gPunctual.pointLight.decay);
		// SpotLight�̓��ˌ�
		float3 spotLightDirectionOnSurface = normalize(input.worldPosition - gPunctual.spotLight.pos);
		// SpotLight�ւ̋���
		float distanceSpotLight = length(gPunctual.spotLight.pos - input.worldPosition);
		// �w���ɂ��R���g���[��
		float factorSpotLight = pow(saturate(-distanceSpotLight / gPunctual.spotLight.distance + 1.0f), gPunctual.spotLight.decay);
		// Camera�����Z�o
		float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
		
		/*-------------------------------------------------------------------------------------------------*/
		/// DirectionalLight

		float3 halfVectorDirectionalLight = normalize(normalize(-gPunctual.directionalLight.direction) + toEye);
		float NDotHDirectionalLight = dot(normalize(input.normal), halfVectorDirectionalLight);
		float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);

		float NdotLDirectionalLight = dot(normalize(input.normal), normalize(-gPunctual.directionalLight.direction));
		float cosDirectionalLight = pow(NdotLDirectionalLight * 0.5f + 0.5f, 2.0f);

		// �g�U����
		float3 diffuseDirectionalLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.directionalLight.color.rgb * cosDirectionalLight * gPunctual.directionalLight.intensity;

		// ���ʔ���
		float3 specularDirectionalLight =
			gPunctual.directionalLight.color.rgb * gPunctual.directionalLight.intensity * specularPowDirectionalLight * gMaterial.specularColor;
		
		/*-------------------------------------------------------------------------------------------------*/
		/// PointLight

		float3 halfVectorPointLight = normalize(-pointLightDirection + toEye);
		float NDotHPointLight = dot(normalize(input.normal), halfVectorPointLight);
		float specularPowPointLight = pow(saturate(NDotHPointLight), gMaterial.shininess);

		float NdotLPointLight = dot(normalize(input.normal), -pointLightDirection);
		float cosPointLight = pow(NdotLPointLight * 0.5f + 0.5f, 2.0f);

		// �g�U����
		float3 diffusePointLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.pointLight.color.rgb * cosPointLight * gPunctual.pointLight.intensity * factorPointLight;

		// ���ʔ���
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

		// �g�U����
		float3 diffuseSpotLight =
			gMaterial.color.rgb * textureColor.rgb * gPunctual.spotLight.color.rgb * cosSpotLight * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight;

		// ���ʔ���
		float3 specularSpotLight =
			gPunctual.spotLight.color.rgb * gPunctual.spotLight.intensity * falloffFactor * factorSpotLight * specularPowSpotLight * gMaterial.specularColor;
		
		output.color.rgb =
			diffuseDirectionalLight + specularDirectionalLight + diffusePointLight + specularPointLight + diffuseSpotLight + specularSpotLight;
		
	} else {
		
		// �e�𗎂Ƃ��O�̐F
		output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	}
	
	// �e������
	output.color.rgb *= shadowMap;
	
	// ���l
	output.color.a = gMaterial.color.a * textureColor.a;

	return output;
}