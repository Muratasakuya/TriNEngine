#include "Player.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Game/System/GameSystem.h>
#include <Game/Utility/GameTimer.h>
#include <Lib/Adapter/JsonAdapter.h>
#include <Lib/Adapter/Easing.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	Player classMethods
//============================================================================*/

void Player::Init() {

	input_ = Input::GetInstance();
	followCamera_ = GameSystem::GameCamera()->GetFollowCamera();

	//========================================================================*/
	//* model *//

	const std::string modelName = "player.gltf";
	animationNames_ = {

		{"walk", "player_Walk"},
		{"dash", "player_Dash"},
	};
	currentAnimationKey_ = "walk";

	BaseAnimationObject::Init(modelName, animationNames_[currentAnimationKey_]);
	BaseAnimationObject::SetMeshRenderer("player");
	parentFolderName_ = "Player/";

	model_->SetTexture("white");

	// InitAnimation
	transform_.SetPlayAnimation(animationNames_[currentAnimationKey_], true);
	transform_.SetNewAnimationData(animationNames_["dash"]);

	ApplyJson();

	isDashing_ = false;
	isOnGround_ = true;

}

void Player::Update() {

	Move(); //* 移動処理

	BaseAnimationObject::Update();

}

void Player::Move() {

	MoveWalk();    // 通常歩き移動
	MoveRequest(); // 移動依頼処理

	if (CheckCurrentMoveBehaviour({ MoveBehaviour::Dash })) {
		// ダッシュ移動
		MoveDash();
	}
	if (CheckCurrentMoveBehaviour({ MoveBehaviour::Jump })) {
		// ジャンプ
		MoveJump();
	}

	RotateToDirection();

}

void Player::MoveRequest() {

	// Rでダッシュ
	if (input_->PushGamepadButton(InputGamePadButtons::RIGHT_SHOULDER)) {

		moveBehaviour_ = MoveBehaviour::Dash;
	} else {
		if (isDashing_) {

			currentMoveBehaviours_.erase(MoveBehaviour::Dash);
			isDashing_ = false;
		}
	}

	// Aでジャンプ
	if (isOnGround_ && input_->TriggerGamepadButton(InputGamePadButtons::A)) {

		moveBehaviour_ = MoveBehaviour::Jump;
	}

	if (moveBehaviour_) {

		// 同じ値はセットできないようにする
		if (!CheckCurrentMoveBehaviour({ *moveBehaviour_ })) {

			currentMoveBehaviours_.insert(*moveBehaviour_);
		}

		moveBehaviour_ = std::nullopt;
	}
}

void Player::MoveWalk() {

	// ダッシュ中はこの処理を行わない
	if (isDashing_) {
		return;
	}

	Vector2 leftStickVal = input_->GetLeftStickVal();

	if (std::fabs(leftStickVal.x) > FLT_EPSILON || std::fabs(leftStickVal.y) > FLT_EPSILON) {

		// 入力がある場合のみ速度を計算する
		Vector3 inputDirection(leftStickVal.x, 0.0f, leftStickVal.y);
		inputDirection = Vector3::Normalize(inputDirection);

		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(followCamera_->GetRotate());
		Vector3 rotatedDirection = Vector3::TransferNormal(inputDirection, rotateMatrix);
		rotatedDirection = Vector3::Normalize(rotatedDirection);

		move_ = rotatedDirection * velocity_;
	} else {

		// 入力がなければどんどん減速させる
		move_ *= moveDecay_;
		// 一定の速度以下で止まる
		if (move_.Length() < 0.001f) {
			move_.Init();
		}
	}

	transform_.translation.x += move_.x;
	transform_.translation.z += move_.z;

}

void Player::MoveDash() {

	// 補間処理をダッシュ中のみに限定
	if (!isDashing_) {

		dashSpeed_.lerpTimer = 0.0f;
		dashSpeed_.current = dashSpeed_.start;
		isDashing_ = true;
	}

	// 補間処理
	if (dashSpeed_.lerpTimer <= dashSpeed_.lerpTime) {

		dashSpeed_.lerpTimer += GameTimer::GetScaledDeltaTime();
		float t = dashSpeed_.lerpTimer / dashSpeed_.lerpTime;
		float eased = EaseOutBack(t);

		dashSpeed_.current = std::lerp(dashSpeed_.start, dashSpeed_.end, eased);
	}

	Vector2 leftStickVal = input_->GetLeftStickVal();

	if (fabs(leftStickVal.x) > FLT_EPSILON || fabs(leftStickVal.y) > FLT_EPSILON) {

		// ダッシュ中の方向を維持
		Vector3 inputDirection(leftStickVal.x, 0.0f, leftStickVal.y);
		inputDirection = Vector3::Normalize(inputDirection);

		Matrix4x4 rotateMatrix = Matrix4x4::MakeRotateMatrix(followCamera_->GetRotate());
		Vector3 rotatedDirection = Vector3::TransferNormal(inputDirection, rotateMatrix);
		rotatedDirection = Vector3::Normalize(rotatedDirection);

		move_ = rotatedDirection * velocity_ * dashSpeed_.current;
	} else {

		// 入力がない場合でもダッシュ速度を維持
		move_ *= 1.0f;
	}

	transform_.translation.x += move_.x;
	transform_.translation.z += move_.z;

}

void Player::MoveJump() {

	if (isOnGround_) {

		// ジャンプ初速度設定
		velocity_.y = jumpStrength_;
		isOnGround_ = false;
	}

	if (!isOnGround_) {

		velocity_.y += gravity * GameTimer::GetScaledDeltaTime();
		transform_.translation.y += velocity_.y * GameTimer::GetScaledDeltaTime();

		if (transform_.translation.y <= groundY) {

			// ここでジャンプ終了
			transform_.translation.y = 0.0f;
			velocity_.y = 0.0f;
			isOnGround_ = true;

			currentMoveBehaviours_.erase(MoveBehaviour::Jump);
		}
	}

}

void Player::DerivedImGui() {

	if (ImGui::Button("Save")) {
		SaveJson();
	}

	ImGui::Separator();

	// 現在のMoveBehaviour
	ImGui::Text("Current Move Behaviours:");
	if (currentMoveBehaviours_.empty()) {
		ImGui::Text("  None");
	} else {
		for (const auto& behaviour : currentMoveBehaviours_) {
			switch (behaviour) {
			case MoveBehaviour::Dash:
				ImGui::BulletText("Dash");
				break;
			case MoveBehaviour::Jump:
				ImGui::BulletText("Jump");
				break;
			default:
				ImGui::BulletText("Unknown");
				break;
			}
		}
	}
	ImGui::Separator();

	if (ImGui::BeginTabBar("PlayerTabBar")) {

		// MovementTab
		if (ImGui::BeginTabItem("Movement")) {

			ImGui::DragFloat("rotationLerpRate", &rotationLerpRate_, 0.01f);
			ImGui::DragFloat("moveDecay", &moveDecay_, 0.01f);
			ImGui::DragFloat3("moveVelocity", &velocity_.x, 0.01f);
			ImGui::DragFloat("dashSpeed.start", &dashSpeed_.start, 0.01f);
			ImGui::DragFloat("dashSpeed.end", &dashSpeed_.end, 0.01f);
			ImGui::DragFloat("dashSpeed.lerpTime", &dashSpeed_.lerpTime, 0.01f);
			ImGui::Text("currentDahSpeed: %4.1f", dashSpeed_.current);
			ImGui::DragFloat("jumpStrength", &jumpStrength_, 0.01f);

			ImGui::EndTabItem();
		}
		// AnimationTab
		if (ImGui::BeginTabItem("Animation")) {
			
			ImGui::DragFloat("animationDuration", &animationDuration_, 0.01f);
			if (ImGui::BeginCombo("Animations", currentAnimationKey_.c_str())) {
				for (auto& kv : animationNames_) {

					const std::string& key = kv.first;
					const bool isSelected = (currentAnimationKey_ == key);

					if (ImGui::Selectable(key.c_str(), isSelected)) {

						currentAnimationKey_ = key;
						BaseAnimationObject::SwitchAnimation(animationNames_[currentAnimationKey_], false, animationDuration_);
					}

					if (isSelected) {

						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::Separator();
			transform_.AnimationInfo();

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

}

void Player::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_.value() + GetName() + "EditParameter.json");

	rotationLerpRate_ = data["rotationLerpRate"];
	moveDecay_ = data["moveDecay"];
	velocity_ = JsonAdapter::ToVector3(data["moveVelocity"]);
	dashSpeed_.start = data["dashSpeed_start"];
	dashSpeed_.end = data["dashSpeed_end"];
	dashSpeed_.lerpTime = data["dashSpeed_lerpTime"];
	jumpStrength_ = data["jumpStrength"];
	animationDuration_ = data["animationDuration"];

}

void Player::SaveJson() {

	Json data;

	data["rotationLerpRate"] = rotationLerpRate_;
	data["moveDecay"] = moveDecay_;
	data["moveVelocity"] = JsonAdapter::FromVector3(velocity_);
	data["dashSpeed_start"] = dashSpeed_.start;
	data["dashSpeed_end"] = dashSpeed_.end;
	data["dashSpeed_lerpTime"] = dashSpeed_.lerpTime;
	data["jumpStrength"] = jumpStrength_;
	data["animationDuration"] = animationDuration_;

	JsonAdapter::Save(parentFolderName_.value() + GetName() + "EditParameter.json", data);

}

void Player::RotateToDirection() {

	Vector3 direction = Vector3(move_.x, 0.0f, move_.z).Normalize();

	if (direction.Length() <= 0.0f) {
		return;
	}

	Quaternion targetRotation = Quaternion::LookRotation(direction, Direction::Up());
	transform_.rotation = Quaternion::Slerp(transform_.rotation, targetRotation, rotationLerpRate_);
}

bool Player::CheckCurrentMoveBehaviour(std::initializer_list<MoveBehaviour> states) {

	for (auto state : states) {
		if (currentMoveBehaviours_.find(state) == currentMoveBehaviours_.end()) {

			return false;
		}
	}
	return true;
}