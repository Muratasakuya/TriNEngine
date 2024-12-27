#include "Object3D.hlsli"

/*===========================================================
                     Object3D VS Shader
===========================================================*/

struct TransformationMatrix {

	float4x4 World;
	float4x4 WVP;
	float4x4 WorldInverseTranspose;
};

struct ShadowLight {
	
	float4x4 lightViewProjection;
};

struct VertexShaderInput {
    
	float4 position : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : NORMAL0;
};

ConstantBuffer<TransformationMatrix> gTransformationMatirx : register(b0);
ConstantBuffer<ShadowLight> gShadowLight : register(b1);

VertexShaderOutput main(VertexShaderInput input) {

	VertexShaderOutput output;

	output.position = mul(input.position, gTransformationMatirx.WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatirx.WorldInverseTranspose));
	
	output.worldPosition = mul(input.position, gTransformationMatirx.World).xyz;
	
	float4 worldPos = float4(output.worldPosition, 1.0f);
	output.positionInLVP = mul(worldPos, gShadowLight.lightViewProjection);

	return output;
}