#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Collision/Collider.h>

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

	//========================================================================*/
	//* functions

	// json
	void ApplyJson();
	void SaveJson();

};