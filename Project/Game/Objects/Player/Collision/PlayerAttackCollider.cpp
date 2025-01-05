#include "PlayerAttackCollider.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Objects/Player/Player.h>
#include <Lib/Adapter/JsonAdapter.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	PlayerAttackCollider classMethods
//============================================================================*/

void PlayerAttackCollider::Init(Player* player) {

	player_ = player;

	// Collider設定
	Collider::SetCollisionShapeOBB(); //* OBBの形で設定

	// 自身のタイプと攻撃対象
	Collider::type_ = ColliderType::Type_PlayerAttack;
	Collider::targetType_ = ColliderType::Type_Enemy;
	// Debug
	Collider::name_ = "playerAttack";

	ApplyJson();

}

void PlayerAttackCollider::Update() {

	// OBBの更新
	Collider::rotate_ = player_->GetWorldTransform().rotation; //* 回転はそのまま

	Vector3 forward = player_->GetWorldTransform().GetForward();
	forward.Normalize();

	// 最初の攻撃の時
	if (player_->IsWaitToFirstAttack() &&
		player_->GetWorldTransform().GetAnimationProgress() >= 0.4f) {

		// オフセット計算
		Vector3 offset = forward * firstParameter_.offsetForward;
		offset.y += firstParameter_.offsetY;

		Collider::centerPos_ = player_->GetWorldTransform().translation + offset;
		Collider::size_ = firstParameter_.size;
	}
	// 2回目の攻撃の時
	else if (player_->IsWaitToSecondAttack() &&
		player_->GetWorldTransform().GetAnimationProgress() >= 0.4f) {

		// オフセット計算
		Vector3 offset = forward * secondParameter_.offsetForward;
		offset.y += secondParameter_.offsetY;

		Collider::centerPos_ = player_->GetWorldTransform().translation + offset;
		Collider::size_ = secondParameter_.size;
	}
	// 3回目の攻撃
	else if (player_->IsWaitToThirdAttack() &&
		player_->GetWorldTransform().GetAnimationProgress() >= 0.76f) {

		// オフセット計算
		Vector3 offset = forward * thirdParameter_.offsetForward;
		offset.y += thirdParameter_.offsetY;

		Collider::centerPos_ = player_->GetWorldTransform().translation + offset;
		Collider::size_ = thirdParameter_.size;
	} else {

		Collider::centerPos_.y = -8.0f;
		Collider::size_.Init();
	}

	Collider::OBBUpdate();

}

void PlayerAttackCollider::ImGui() {

	if (ImGui::Button("Save##Collision")) {

		SaveJson();
	}

	ImGui::Separator();
	ImGui::Text("WaitToFirst");

	ImGui::DragFloat("offsetY##first", &firstParameter_.offsetY, 0.01f);
	ImGui::DragFloat("offsetForward##first", &firstParameter_.offsetForward, 0.01f);
	ImGui::DragFloat3("size##first", &firstParameter_.size.x, 0.01f);

	ImGui::Separator();
	ImGui::Text("WaitToSecond");

	ImGui::DragFloat("offsetY##second", &secondParameter_.offsetY, 0.01f);
	ImGui::DragFloat("offsetForward##second", &secondParameter_.offsetForward, 0.01f);
	ImGui::DragFloat3("size##second", &secondParameter_.size.x, 0.01f);

	ImGui::Separator();
	ImGui::Text("WaitToThird");

	ImGui::DragFloat("offsetY##third", &thirdParameter_.offsetY, 0.01f);
	ImGui::DragFloat("offsetForward##third", &thirdParameter_.offsetForward, 0.01f);
	ImGui::DragFloat3("size##third", &thirdParameter_.size.x, 0.01f);

}

void PlayerAttackCollider::ApplyJson() {

	Json data = JsonAdapter::Load("Player/" + GetName() + "CollisionParameter.json");

	firstParameter_.offsetY = data["firstParameter_offsetY"];
	firstParameter_.offsetForward = data["firstParameter_offsetForward"];
	firstParameter_.size = JsonAdapter::ToVector3(data["firstParameter_size"]);

	secondParameter_.offsetY = data["secondParameter_offsetY"];
	secondParameter_.offsetForward = data["secondParameter_offsetForward"];
	secondParameter_.size = JsonAdapter::ToVector3(data["secondParameter_size"]);

	thirdParameter_.offsetY = data["thirdParameter_offsetY"];
	thirdParameter_.offsetForward = data["thirdParameter_offsetForward"];
	thirdParameter_.size = JsonAdapter::ToVector3(data["thirdParameter_size"]);

}

void PlayerAttackCollider::SaveJson() {

	Json data;

	data["firstParameter_offsetY"] = firstParameter_.offsetY;
	data["firstParameter_offsetForward"] = firstParameter_.offsetForward;
	data["firstParameter_size"] = JsonAdapter::FromVector3(firstParameter_.size);

	data["secondParameter_offsetY"] = secondParameter_.offsetY;
	data["secondParameter_offsetForward"] = secondParameter_.offsetForward;
	data["secondParameter_size"] = JsonAdapter::FromVector3(secondParameter_.size);

	data["thirdParameter_offsetY"] = thirdParameter_.offsetY;
	data["thirdParameter_offsetForward"] = thirdParameter_.offsetForward;
	data["thirdParameter_size"] = JsonAdapter::FromVector3(thirdParameter_.size);

	JsonAdapter::Save("Player/" + GetName() + "CollisionParameter.json", data);
}