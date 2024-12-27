
/*===========================================================
                   PrimitiveLine VS Shader
===========================================================*/

struct Camera {
	
	float4x4 viewProjectionMatrix;
};

struct VertexShaderOutput {
	
	float4 position : SV_POSITION;
};

struct VertexShaderInput {

	float4 position : POSITION0;
};

ConstantBuffer<Camera> gCamera : register(b0);

VertexShaderOutput main(VertexShaderInput input) {
	
	VertexShaderOutput output;
	output.position = mul(input.position, gCamera.viewProjectionMatrix);

	return output;
}