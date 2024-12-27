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
			textureColor.xyz *= 0.25f;
		}
	}
	
	output.color.rgb = gMaterial.color.rgb * textureColor.rgb;
	output.color.rgb *= shadowMap;
	output.color.a = gMaterial.color.a * textureColor.a;

	return output;
}