#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {

	float4 color : SV_TARGET0;
};

// ConstBuffer�œn���ׂ�
// ���S�_�A��������ɕ��ˏ�Ƀu���[��������
static const float2 kCenter = float2(0.5f, 0.5f);
// �T���v�����O���A�����قǊ��炩�����Ǐd��
static const int kNumSamples = 10;
// �ڂ����̕��A�傫���قǑ傫��
static const float kBlurWidth = 0.01f;

PixelShaderOutput main(VertexShaderOutput input) {

	PixelShaderOutput output;
	output.color = gTexture.Sample(gSampler, input.texcoord);
	
	// ���S���猻�݂�uv�ɑ΂��Ă̕������v�Z
	// ������Ή����قǉ������T���v�����O����A�Ȃ̂Ő��K�����Ȃ�
	float2 direction = input.texcoord - kCenter;
	float3 outputColor = float3(0.0f, 0.0f, 0.0f);
	
	for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex) {
		
		// ���݂�uv�������v�Z���������ɃT���v�����O�_��i�߂Ȃ���T���v�����O���Ă���
		float2 texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);
		texcoord = clamp(texcoord, float2(0.0f, 0.0f), float2(0.9f, 0.9f));
		outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
	}
	
	// ����������
	outputColor.rgb *= rcp(float(kNumSamples));
	output.color.rgb = outputColor;
	output.color.a = 1.0f;
	
	return output;
}