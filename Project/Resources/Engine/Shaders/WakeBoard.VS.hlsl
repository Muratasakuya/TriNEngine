#include "WakeBoard.hlsli"

struct TransformationMatrix {

	float4x4 World;
	float4x4 WVP;
	float4x4 WorldInverseTranspose;
};

struct WaveParameters {
	
	float time;       // 時間変数
	float amplitude;  // 波の振幅
	float frequency;  // 波の周波数
	float speed;      // 波の速度
	float wavelength; // 波長
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

	// 頂点位置の変位
	float3 displacedPos = input.position.xyz;

	// X軸とZ軸方向の波の高さの計算
	float waveHeightX =
	gWaveParameters.amplitude * sin((displacedPos.x / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	float waveHeightZ =
	gWaveParameters.amplitude * sin((displacedPos.z / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;

	// Y軸の合成された波の高さ
	displacedPos.y += waveHeightX + waveHeightZ;
	float4 transformedPosition = float4(displacedPos, 1.0f);

	// クリップ空間変換
	output.position = mul(transformedPosition, gTransformationMatrix.WVP);
	
	//===================================================================*/
	/// Texcoord

	// ベースの1番下のテクスチャはそのまま
	output.texcoordBase = input.texcoord;

	// 青波のUV
	output.texcoordBlue = input.texcoord;
	output.texcoordBlue.x += sin(gWaveParameters.time * 0.15f) * 0.15f;
	output.texcoordBlue.y += cos(gWaveParameters.time * 0.15f) * 0.15f;

	// 白波のUV
	output.texcoordWhite = input.texcoord;
	output.texcoordWhite.x -= sin(gWaveParameters.time * 0.15f) * 0.15f;
	output.texcoordWhite.y -= cos(gWaveParameters.time * 0.15f) * 0.15f;
	
	//===================================================================*/
	/// Normal

	// 法線の計算に使う微小変位
	float epsilon = 0.01f;
	
	///* X、Zを微小変位させた位置を計算 *///

	// X軸方向
	float3 displacedPosX = displacedPos;
	displacedPosX.x += epsilon;
	float waveHeightX1 = gWaveParameters.amplitude * sin((displacedPosX.x / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	displacedPosX.y = waveHeightX1 + waveHeightZ;
	// Z軸方向
	float3 displacedPosZ = displacedPos;
	displacedPosZ.z += epsilon;
	float waveHeightZ1 = gWaveParameters.amplitude * sin((displacedPosZ.z / gWaveParameters.wavelength + gWaveParameters.time * gWaveParameters.speed)) * gWaveParameters.frequency;
	displacedPosZ.y = waveHeightX + waveHeightZ1;

	// 法線ベクトルの計算
	float3 tangentX = normalize(displacedPosX - displacedPos);
	float3 tangentZ = normalize(displacedPosZ - displacedPos);
	float3 normal = normalize(cross(tangentZ, tangentX));

	// 法線の再計算
	output.normal = normalize(mul(normal, (float3x3) gTransformationMatrix.WorldInverseTranspose));
	
	// ワールド座標の計算
	output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;

	return output;
}