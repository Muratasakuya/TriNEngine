#include "WakeBoard.hlsli"

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

ConstantBuffer<Material> gMaterial : register(b0);             // ���W�X�^ 0
ConstantBuffer<PunctualLight> gPunctual : register(b1);        // ���W�X�^ 1
ConstantBuffer<Camera> gCamera : register(b2);                 // ���W�X�^ 2

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

	// ���Z
	float4 combinedBlueWaves = blueWaveColor1 - blueWaveColor2;

	// �F�̍���
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