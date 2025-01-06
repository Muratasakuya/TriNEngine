#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Particle/BaseParticle.h>

//============================================================================*/
//	HitLineParticle Type
//============================================================================*/
enum class HitLineParticleType {

	RedWhite, // 白赤い線
	Red,      // ほぼ赤

	Count,    // Typeの数
};
static const inline uint32_t hitLineTypeNum =
static_cast<uint32_t>(HitLineParticleType::Count);

//============================================================================*/
//	HitLineParticle class
//============================================================================*/
class HitLineParticle
	:public BaseParticle<kDispersion> {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	HitLineParticle(HitLineParticleType type);
	~HitLineParticle() = default;

	void Init() override;

	void Update() override;

	void EmitOnce(const Vector3& translate);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	HitLineParticleType type_;

};

