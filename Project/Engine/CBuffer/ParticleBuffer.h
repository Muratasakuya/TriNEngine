#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Lib/MathUtils/Vector4.h>

// c++
#include <cstdint>
#include <vector>

//============================================================================*/
//	bufferSize ParticleForGPU
//============================================================================*/

struct ParticleForGPU {

	Matrix4x4 World;
	Matrix4x4 WVP;
	Color color;
};

//============================================================================*/
//	ParticleBuffer class
//============================================================================*/
class ParticleBuffer :
	public DXConstBuffer<ParticleForGPU> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	ParticleBuffer() = default;
	~ParticleBuffer() = default;

	void Init(UINT instanceCount);

	void Update();

	//========================================================================*/
	//* variables

	std::vector<ParticleForGPU> properties;

};