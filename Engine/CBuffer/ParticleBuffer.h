#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Base/DXConstBuffer.h>
#include <Game/3D/Particle/ParticleStructure.h>

// c++
#include <cstdint>
#include <vector>

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