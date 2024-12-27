#include "Fullscreen.hlsli"

// �}���`�p�X�����_�����O�ŏc�A�������Ă��Ƃ߂�����y���Ȃ�炵��

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// 3x3��Index
static const float2 kIndex3x3[3][3] = {
	{ { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
	{ { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
	{ { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } }
};

// ��
static const float PI = 3.14159265f;

// �K�E�X�֐�
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

	// uvStepSize�̎Z�o
	int width, height;
	gTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(rcp(width), rcp(height));

	output.color.rgb = float3(0.0f, 0.0f, 0.0f);
	output.color.a = 1.0f;

	// �J�[�l���ƃE�F�C�g�̌v�Z
	float kernel3x3[3][3];
	float weight = 0.0f;
	// �W���΍� ConstBuffer�œn�����������Ă�����
	float sigma = 2.0f;

	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			
			// �K�E�X�֐���p���ăJ�[�l���̒l���v�Z
			kernel3x3[x][y] = gauss(kIndex3x3[x][y].x, kIndex3x3[x][y].y, sigma);
			weight += kernel3x3[x][y];
		}
	}

	// �J�[�l�����g�p���ď�ݍ��ݏ������s��
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {

			// texcoord�̌v�Z
			float2 texcoord = input.texcoord + kIndex3x3[i][j] * uvStepSize;
			// �e�N�X�`���̃T���v���l�擾
			float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			// �J�[�l���l���|���ĐF�𑫂�
			output.color.rgb += fetchColor * kernel3x3[i][j];
		}
	}

    // ���v�l�𐳋K��
	output.color.rgb *= rcp(weight);

	return output;
}
