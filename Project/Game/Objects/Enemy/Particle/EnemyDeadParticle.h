#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/BaseParticle.h>

//============================================================================*/
//	EnemyDeadParticle class
//============================================================================*/
class EnemyDeadParticle
	:public BaseParticle<kDispersion> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	EnemyDeadParticle(uint32_t index);
	~EnemyDeadParticle() = default;

	void Init() override;

	void Update() override;

	void EmitOnce(const Vector3& translate);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	uint32_t index_;

};