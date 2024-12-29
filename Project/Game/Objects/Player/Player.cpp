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

		// Move
		{"wait", "player_Wait"},
		{"dash", "player_Dash"},
		{"jump", "player_Jump"},

		// Attack
		{"waitToAttack0", "player_WaitToAttack0"},
		{"dashToAttack0", "player_DashToAttack0"},
	};
	currentAnimationKey_ = "wait";

	BaseAnimationObject::Init(modelName, animationNames_[currentAnimationKey_]);
	BaseAnimationObject::SetMeshRenderer("player");
	parentFolderName_ = "Player/";

	model_->SetTexture("white");

	// InitAnimation
	transform_.SetPlayAnimation(animationNames_[currentAnimationKey_], true);
	transform_.SetNewAnimationData(animationNames_["dash"]);
	transform_.SetNewAnimationData(animationNames_["jump"]);
	transform_.SetNewAnimationData(animationNames_["waitToAttack0"]);
	transform_.SetNewAnimationData(animationNames_["dashToAttack0"]);

	ApplyJson();

	isDashing_ = false;
	isJump_ = false;
	isWaitToFirstAttack_ = false;
	isDashToFirstAttack_ = false;

}

void Player::Update() {

	Move(); //* 移動処理

	UpdateAnimation(); //* アニメーション設定処理

	BaseAnimationObject::Update();

}

void Player::UpdateAnimation() {

	if (isDashing_ && move_.Length() > 0.001f) {

		// ダッシュ中のAnimationの設定
		BaseAnimationObject::SwitchAnimation(animationNames_["dash"], false, waitToDashDuration_);
		return;
	}
	if (isJump_ && !isDashing_) {

		// ジャンプAnimationの設定
		BaseAnimationObject::SwitchAnimation(animationNames_["jump"], false, moveToJumpDuration_);
		return;
	}
	if (!isDashToFirstAttack_ && isWaitToFirstAttack_ && !isDashing_) {

		// 待機中からの最初の攻撃Animation
		BaseAnimationObject::SwitchAnimation(animationNames_["waitToAttack0"], false, waitToAttackDuration_);
		return;
	}
	if (isDashToFirstAttack_) {

		// ダッシュからの最初の攻撃Animation
		BaseAnimationObject::SwitchAnimation(animationNames_["dashToAttack0"], false, dashToAttackDuration_);
		return;
	}

	// 何もなければここを通って待機Animationになる
	BaseAnimationObject::SwitchAnimation(animationNames_["wait"], true, moveToWaitDuration_);

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
	if (CheckCurrentMoveBehaviour({ MoveBehaviour::WaitToFirstAttack })) {
		// 待機からの最初の攻撃
		WaitToFirstAttack();
	}
	if (CheckCurrentMoveBehaviour({ MoveBehaviour::DashToFirstAttack })) {
		// ダッシュからの最初の攻撃
		DashToFirstAttack();
	}

	RotateToDirection();

}

void Player::MoveRequest() {

	// Rでダッシュ
	if (!isDashToFirstAttack_ && !isWaitToFirstAttack_ && !isJump_ &&
		input_->PushGamepadButton(InputGamePadButtons::RIGHT_SHOULDER)) {

		moveBehaviour_ = MoveBehaviour::Dash;
	} else {
		// ダッシュ中にRを離したらダッシュを終わらせる
		if (isDashing_) {

			currentMoveBehaviours_.erase(MoveBehaviour::Dash);
			isDashing_ = false;
		}
	}

	// Aでジャンプ
	if (!isDashToFirstAttack_ && !isWaitToFirstAttack_ && !isJump_ &&
		input_->TriggerGamepadButton(InputGamePadButtons::A)) {

		moveBehaviour_ = MoveBehaviour::Jump;
	}
	// Animationが終わればfalseにする
	if (isJump_) {
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::Jump);
			isJump_ = false;
		}
	}

	AttackRequest(); // 攻撃依頼

	// 行動ビヘイビアが何かあればセットする
	if (moveBehaviour_) {

		// 同じ値はセットできないようにする
		if (!CheckCurrentMoveBehaviour({ *moveBehaviour_ })) {

			currentMoveBehaviours_.insert(*moveBehaviour_);
		}

		moveBehaviour_ = std::nullopt;
	}
}

void Player::AttackRequest() {

	//========================================================================*/
	// 待機中の攻撃
	// Xで攻撃
	// ジャンプをしていないかつなにも攻撃をしていないとき
	if (!isJump_ && !isWaitToFirstAttack_ && !isDashToFirstAttack_ &&
		input_->TriggerGamepadButton(InputGamePadButtons::X)) {

		moveBehaviour_ = MoveBehaviour::WaitToFirstAttack;
	}
	// Animationが終わればfalseにする
	if (isWaitToFirstAttack_) {
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::WaitToFirstAttack);
			isWaitToFirstAttack_ = false;
		}
	}
	//========================================================================*/
	// ダッシュ中の攻撃
	// Xで攻撃
	// ジャンプをしていないかつダッシュ中かつダッシュAnimationが終わっている勝つ何も攻撃していないとき
	if (!isJump_ && isDashing_ && !isDashToFirstAttack_ && transform_.AnimationFinish() &&
		input_->TriggerGamepadButton(InputGamePadButtons::X)) {

		moveBehaviour_ = MoveBehaviour::DashToFirstAttack;
		// ダッシュ移動を取り消す
		isDashing_ = false;
		currentMoveBehaviours_.erase(MoveBehaviour::Dash);
	}
	// Animationが終わればfalseにする
	if (isDashToFirstAttack_) {
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::DashToFirstAttack);
			isDashToFirstAttack_ = false;
		}
	}
	//========================================================================*/

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

	// ジャンプアニメーション開始
	isJump_ = true;

}

void Player::WaitToFirstAttack() {

	// 待機からの最初の攻撃アニメーション開始
	isWaitToFirstAttack_ = true;

}

void Player::DashToFirstAttack() {

	// ダッシュからの最初の攻撃アニメーション開始
	isDashToFirstAttack_ = true;

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

			ImGui::EndTabItem();
		}
		// AnimationTab
		if (ImGui::BeginTabItem("Animation")) {

			ImGui::DragFloat("waitToDashDuration", &waitToDashDuration_, 0.01f);
			ImGui::DragFloat("moveToJumpDuration", &moveToJumpDuration_, 0.01f);
			ImGui::DragFloat("moveToWaitDuration", &moveToWaitDuration_, 0.01f);
			ImGui::DragFloat("waitToAttackDuration", &waitToAttackDuration_, 0.01f);
			ImGui::DragFloat("dashToAttackDuration", &dashToAttackDuration_, 0.01f);

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
	waitToDashDuration_ = data["waitToDashDuration"];
	moveToWaitDuration_ = data["moveToWaitDuration"];
	moveToJumpDuration_ = data["moveToJumpDuration"];
	waitToAttackDuration_ = data["waitToAttackDuration"];
	dashToAttackDuration_ = data["dashToAttackDuration"];

}

void Player::SaveJson() {

	Json data;

	data["rotationLerpRate"] = rotationLerpRate_;
	data["moveDecay"] = moveDecay_;
	data["moveVelocity"] = JsonAdapter::FromVector3(velocity_);
	data["dashSpeed_start"] = dashSpeed_.start;
	data["dashSpeed_end"] = dashSpeed_.end;
	data["dashSpeed_lerpTime"] = dashSpeed_.lerpTime;
	data["waitToDashDuration"] = waitToDashDuration_;
	data["moveToWaitDuration"] = moveToWaitDuration_;
	data["moveToJumpDuration"] = moveToJumpDuration_;
	data["waitToAttackDuration"] = waitToAttackDuration_;
	data["dashToAttackDuration"] = dashToAttackDuration_;

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