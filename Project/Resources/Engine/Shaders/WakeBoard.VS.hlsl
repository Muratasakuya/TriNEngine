#include "WakeBoard.hlsli"

struct TransformationMatrix {

	float4x4 World;
	float4x4 WVP;
	float4x4 WorldInverseTranspose;
};

struct WaveParameters {
	
	float time;       // ���ԕϐ�
	float amplitude;  // �g�̐U��
	float frequency;  // �g�̎��g��
	float speed;      // �g�̑��x
	float wavelength; // �g��
};

struct VertexShaderInput {
    
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);
ConstantBuffer<WaveParameters> gWaveParameters : register(b1);

VertexShaderOutput main(VertexShaderInput input) {
	
	VertexShaderOutput output;
	
	//===================================================================*/
	/// Position

	// ���_�ʒu�̕ψ�
	float3 displacedPos = input.position.xyz;

	// X����Z�������̔g�̍����̌v�Z
	float waveHeightX =
	gWaveParameters.amplitude * sin((displacedPos.x / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	float waveHeightZ =
	gWaveParameters.amplitude * sin((displacedPos.z / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;

	// Y���̍������ꂽ�g�̍���
	displacedPos.y += waveHeightX + waveHeightZ;
	float4 transformedPosition = float4(displacedPos, 1.0f);

	// �N���b�v��ԕϊ�
	output.position = mul(transformedPosition, gTransformationMatrix.WVP);
	
	//===================================================================*/
	/// Texcoord

	// �x�[�X��1�ԉ��̃e�N�X�`���͂��̂܂�
	output.texcoordBase = input.texcoord;

	// �g��UV
	output.texcoordBlue = input.texcoord;
	output.texcoordBlue.x += sin(gWaveParameters.time * 0.15f) * 0.15f;
	output.texcoordBlue.y += cos(gWaveParameters.time * 0.15f) * 0.15f;

	// ���g��UV
	output.texcoordWhite = input.texcoord;
	output.texcoordWhite.x -= sin(gWaveParameters.time * 0.15f) * 0.15f;
	output.texcoordWhite.y -= cos(gWaveParameters.time * 0.15f) * 0.15f;
	
	//===================================================================*/
	/// Normal

	// �@���̌v�Z�Ɏg�������ψ�
	float epsilon = 0.01f;
	
	///* X�AZ������ψʂ������ʒu���v�Z *///

	// X������
	float3 displacedPosX = displacedPos;
	displacedPosX.x += epsilon;
	float waveHeightX1 = gWaveParameters.amplitude * sin((displacedPosX.x / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	displacedPosX.y = waveHeightX1 + waveHeightZ;
	// Z������
	float3 displacedPosZ = displacedPos;
	displacedPosZ.z += epsilon;
	float waveHeightZ1 = gWaveParameters.amplitude * sin((displacedPosZ.z / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	displacedPosZ.y = waveHeightX + waveHeightZ1;

	// �@���x�N�g���̌v�Z
	float3 tangentX = normalize(displacedPosX - displacedPos);
	float3 tangentZ = normalize(displacedPosZ - displacedPos);
	float3 normal = normalize(cross(tangentZ, tangentX));

	// �@���̍Čv�Z
	output.normal = normalize(mul(normal, (float3x3) gTransformationMatrix.WorldInverseTranspose));
	
	// ���[���h���W�̌v�Z
	output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;

	return output;
}