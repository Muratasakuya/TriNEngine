#include "Fullscreen.hlsli"

/// �}�e���A���\����
struct Material {
	
	float4x4 projectionInverse;
};

struct PixelShaderOutput {
	
	float4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);

// Render
Texture2D<float4> gRenderTexture : register(t0);
SamplerState gSamplerLinear : register(s0);
// Depth
Texture2D<float> gDepthTexture : register(t1);
SamplerState gSamplerPoint : register(s1);

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
	gRenderTexture.GetDimensions(width, height);
	float2 uvStepSize = float2(1.0f / width, 1.0f / height);

    // �c�����ꂼ��̏�ݍ��݌��ʂ��i�[����
	float2 difference = float2(0.0f, 0.0f);
    // �F���P�x�ɕϊ����āA��ݍ��݂��s���Ă���
	for (int x = 0; x < 3; ++x) {
		for (int y = 0; y < 3; ++y) {

			float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
			float ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord).r;
            // NDC -> view�BP^{-1}�ɂ�����x��y��zw�ɉe����^���Ȃ�
			float4 viewSpace = mul(float4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            // �������W�n����f�J���g���W�n�֕ϊ�
			float viewZ = viewSpace.z * rcp(viewSpace.w);
			difference.x += viewZ * kPrewittHorizontalKernel[x][y];
			difference.y += viewZ * kPrewittVerticalKernel[x][y];
		}
	}

    // �ω��̒������E�F�C�g�Ƃ��č���
	float weight = length(difference);
	weight = saturate(weight);

	output.color.rgb = (1.0f - weight) * gRenderTexture.Sample(gSamplerLinear, input.texcoord).rgb;
	output.color.a = 1.0f;

	return output;
}