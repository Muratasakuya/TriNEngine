#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/BaseParticle.h>

//============================================================================*/
//	HitStarParticle class
//============================================================================*/
class HitStarParticle
	:public BaseParticle<kDispersion> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	HitStarParticle() = default;
	~HitStarParticle() = default;

	void Init() override;

	void Update() override;

	void EmitOnce(const Vector3& translate);

};