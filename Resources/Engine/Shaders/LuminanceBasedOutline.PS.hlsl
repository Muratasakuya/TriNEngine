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

// ��
static const float kPrewittHorizontalKernel[3][3] = {
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
	{ -1.0f / 6.0f, 0.0f, 1.0f / 6.0f }
};
// �c
static const float kPrewittVerticalKernel[3][3] = {
	{ -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f }
};

// RGB -> �P�x
float Luminance(float3 v) {
	return dot(v, float3(0.2125f, 0.7145f, 0.0721f));
}

PixelShaderOutput main(VertexShaderOutput input) {
	
	PixelShaderOutput output;
	
	// uvStepSize�̎Z�o
	int width, height;
	gTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(rcp(width), rcp(height));

	// �c�����ꂼ��̏�ݍ��݌��ʂ��i�[����
	float2 difference = float2(0.0f, 0.0f);
	// �F���P�x�ɕϊ����āA��ݍ��݂��s���Ă����B����Filter�p��Kernel�ɂȂ��Ă���̂ŁA��邱�Ǝ��͍̂��܂ł̏�ݍ��݂Ɠ���
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {
			
			float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
			float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
			float luminance = Luminance(fetchColor);
			
			difference.x += luminance * kPrewittHorizontalKernel[x][y];
			difference.y += luminance * kPrewittVerticalKernel[x][y];
		}
	}
	
	// �ω��̒������E�F�C�g�Ƃ��č����B�E�F�C�g�̌�����@���F�X�ƍl������B�Ⴆ��difference.x�����g���Ή������̃G�b�W�����o�����
	float weight = length(difference);
	// �Ƃ肠����6�{�AConstBuffer�Œ����p�����[�^�Ƃ��đ���̂��x�X�g
	weight = saturate(weight * 6.0f);
	
	output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler,input.texcoord).rgb;
	output.color.a = 1.0f;

	return output;
}