#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Collision/Collider.h>

// particle
#include <Game/Objects/Player/Particle/HitLineParticle.h>
#include <Game/Objects/Player/Particle/HitStarParticle.h>

// c++
#include <array>

// front
class Player;

//============================================================================*/
//	PlayerAttackCollider class
//============================================================================*/
class PlayerAttackCollider :
	public Collider {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	PlayerAttackCollider() = default;
	~PlayerAttackCollider() = default;

	void Init(Player* player);

	void Update();

	void ImGui();

	//* collision *//

	void OnCollisionEnter([[maybe_unused]] Collider* other) override;

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* structure

	// 攻撃ごとのParameter
	struct Parameter {

		float offsetY;       // yをずらす
		float offsetForward; // 前方方向のオフセット
		Vector3 size;        // 大きさ
	};

	//========================================================================*/
	//* variables

	Player* player_;

	// 各攻撃ごとのParameter
	Parameter firstParameter_;
	Parameter secondParameter_;
	Parameter thirdParameter_;

	//* particle *//

	std::array<std::unique_ptr<HitLineParticle>, hitLineTypeNum> hitParticles_;

	std::unique_ptr<HitStarParticle> hitStarParticle_;

	//========================================================================*/
	//* functions

	// json
	void ApplyJson();
	void SaveJson();

};