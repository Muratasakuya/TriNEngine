#include "Player.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Game/Objects/Enemy/Manager/EnemyManager.h>
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

		// 待機、移動
		{"wait", "player_Wait"},
		{"dash", "player_Dash"},
		{"jump", "player_Jump"},

		// 待機状態からの攻撃コンボ
		{"waitToAttack0", "player_WaitToAttack0"},
		{"waitToAttack1", "player_WaitToAttack1"},
		{"waitToAttack2", "player_WaitToAttack2"},
	};
	currentAnimationKey_ = "wait"; //* 初期状態を設定

	BaseAnimationObject::Init(modelName, animationNames_[currentAnimationKey_]);
	BaseAnimationObject::SetMeshRenderer("player");

	// Light
	BaseAnimationObject::SetBlinnPhongLightingEnable(true);
	// material
	materials_.front().properties.phongRefShininess = 8.0f;

	parentFolderName_ = "Player/";
	model_->SetTexture("white");

	// InitAnimation
	transform_.SetPlayAnimation(animationNames_[currentAnimationKey_], true);
	transform_.SetNewAnimationData(animationNames_["dash"]);
	transform_.SetNewAnimationData(animationNames_["jump"]);
	transform_.SetNewAnimationData(animationNames_["waitToAttack0"]);
	transform_.SetNewAnimationData(animationNames_["waitToAttack1"]);
	transform_.SetNewAnimationData(animationNames_["waitToAttack2"]);

	BaseAnimationObject::ApplyJsonForTransform(transform_);
	ApplyJson();

	posClamped_ = false;

	isDashing_ = false;
	isJump_ = false;

	isWaitToFirstAttack_ = false;
	isWaitToSecondAttackEnable_ = false;
	isWaitToThirdAttackEnable_ = false;

	firstAttackCollisionEnable_ = false;
	secondAttackCollisionEnable_ = false;
	thirdAttackCollisionEnable_ = false;

	//========================================================================*/
	//* collision *//

	isDrawDebugCollider_ = true;

	attackCollider_ = std::make_unique<PlayerAttackCollider>();
	attackCollider_->Init(this);

}

void Player::Update() {

	Move(); //* 移動処理

	UpdateAnimation(); //* アニメーション設定処理

	UpdateCollisionEnable(); //* 衝突フラグ更新

	BaseAnimationObject::Update();

	attackCollider_->Update();

}

void Player::Draw(RendererPipelineType pipeline) {

	BaseAnimationObject::Draw(pipeline);

	if (isDrawDebugCollider_) {
		attackCollider_->DrawCollider();
	}

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
	if (isWaitToFirstAttack_ && !isDashing_) {

		// 待機中からの最初の攻撃Animation
		BaseAnimationObject::SwitchAnimation(animationNames_["waitToAttack0"], false, waitToAttackDuration_);
		return;
	}
	if (isWaitToSecondAttackEnable_ && !isWaitToFirstAttack_) {

		// 待機中からの2回目の攻撃Animation
		BaseAnimationObject::SwitchAnimation(animationNames_["waitToAttack1"], false, waitToSecondAttackDuration_);
		return;
	}
	if (isWaitToThirdAttackEnable_ && !isWaitToSecondAttackEnable_) {

		// 待機中からの3回目の攻撃Animation
		BaseAnimationObject::SwitchAnimation(animationNames_["waitToAttack2"], false, waitToThirdAttackDuration_);
		return;
	}

	// 何もなければここを通って待機Animationになる
	BaseAnimationObject::SwitchAnimation(animationNames_["wait"], true, moveToWaitDuration_);

}

void Player::MoveClamp() {

	// 座標制限、Yはない
	const Vector3 clampPos = Vector3(30.0f, 0.0f, 30.0f);

	Vector3 originalPos = transform_.translation;

	transform_.translation.x = std::clamp(transform_.translation.x, -clampPos.x, clampPos.x);
	transform_.translation.z = std::clamp(transform_.translation.z, -clampPos.z, clampPos.z);

	posClamped_ = transform_.translation != originalPos;

}

void Player::Move() {

	MoveClamp(); // 移動処理

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

	RotateToDirection();

}

void Player::MoveRequest() {

	// Rでダッシュ
	if (!isWaitToFirstAttack_ && !isWaitToSecondAttackEnable_ && !isWaitToThirdAttackEnable_ &&
		!isJump_ && input_->PushGamepadButton(InputGamePadButtons::RIGHT_SHOULDER)) {

		moveBehaviour_ = MoveBehaviour::Dash;
	} else {
		// ダッシュ中にRを離したらダッシュを終わらせる
		if (isDashing_) {

			currentMoveBehaviours_.erase(MoveBehaviour::Dash);
			isDashing_ = false;
		}
	}

	// Aでジャンプ
	if (!isJump_ && input_->TriggerGamepadButton(InputGamePadButtons::A)) {

		moveBehaviour_ = MoveBehaviour::Jump;
		// ダッシュ移動を取り消す
		isDashing_ = false;
		currentMoveBehaviours_.erase(MoveBehaviour::Dash);
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
	if (!isJump_ && !isWaitToFirstAttack_ && !isWaitToSecondAttackEnable_ && !isWaitToThirdAttackEnable_ &&
		input_->TriggerGamepadButton(InputGamePadButtons::X)) {

		moveBehaviour_ = MoveBehaviour::WaitToFirstAttack;
	}
	// Animationが終わればfalseにする
	if (isWaitToFirstAttack_) {
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::WaitToFirstAttack);
			isWaitToFirstAttack_ = false;

			// 最初の攻撃が終わったときに次の攻撃フラグがたっていれば
			if (isWaitToSecondAttackEnable_) {

				moveBehaviour_ = MoveBehaviour::WaitToSecondAttack;
				return;
			}
		}

		// 2回目の攻撃予約
		if (input_->TriggerGamepadButton(InputGamePadButtons::X)) {

			isWaitToSecondAttackEnable_ = true;
		}
	}

	// 最初の攻撃が終わり、次の攻撃が始まったとき
	if (!isWaitToFirstAttack_ && isWaitToSecondAttackEnable_) {

		// Animationが終わればfalseにする
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::WaitToSecondAttack);
			isWaitToSecondAttackEnable_ = false;

			// 最初の攻撃が終わったときに次の攻撃フラグがたっていれば
			if (isWaitToThirdAttackEnable_) {

				moveBehaviour_ = MoveBehaviour::WaitToThirdAttack;
				return;
			}
		}

		// 3回目の攻撃予約
		if (input_->TriggerGamepadButton(InputGamePadButtons::X)) {

			isWaitToThirdAttackEnable_ = true;
		}
	}

	// 2回目の攻撃が終わり、次の攻撃が始まったとき
	// Animationが終わればfalseにする
	if (!isWaitToSecondAttackEnable_ && isWaitToThirdAttackEnable_) {
		if (transform_.AnimationFinish()) {

			currentMoveBehaviours_.erase(MoveBehaviour::WaitToThirdAttack);
			isWaitToThirdAttackEnable_ = false;
		}
	}

}

void Player::MoveWalk() {

	// ダッシュ中はこの処理を行わない
	if (isDashing_) {
		return;
	}
	// 攻撃中は移動不可
	if (isWaitToFirstAttack_ || isWaitToSecondAttackEnable_ || isWaitToThirdAttackEnable_) {
		return;
	}
	// ゲームが終了した後も動けない
	if (enemyManager_->IsFinish()) {
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

	// ゲームが終了した後も動けない
	if (enemyManager_->IsFinish()) {
		return;
	}

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
			case MoveBehaviour::WaitToFirstAttack:
				ImGui::BulletText("WaitToFirstAttack");
				break;
			case MoveBehaviour::WaitToSecondAttack:
				ImGui::BulletText("WaitToSecondAttack");
				break;
			case MoveBehaviour::WaitToThirdAttack:
				ImGui::BulletText("WaitToThirdAttack");
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
			ImGui::DragFloat("waitToSecondAttackDuration", &waitToSecondAttackDuration_, 0.01f);
			ImGui::DragFloat("waitToThirdAttackDuration", &waitToThirdAttackDuration_, 0.01f);

			ImGui::Separator();

			transform_.AnimationInfo();

			ImGui::EndTabItem();
		}
		// CollisionTab
		if (ImGui::BeginTabItem("AttackCollider")) {

			ImGui::Checkbox("isDrawDebugCollider", &isDrawDebugCollider_);
			attackCollider_->ImGui();

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
	waitToSecondAttackDuration_ = data["waitToSecondAttackDuration"];
	waitToThirdAttackDuration_ = data["waitToThirdAttackDuration"];

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
	data["waitToSecondAttackDuration"] = waitToSecondAttackDuration_;
	data["waitToThirdAttackDuration"] = waitToThirdAttackDuration_;

	JsonAdapter::Save(parentFolderName_.value() + GetName() + "EditParameter.json", data);

}

void Player::UpdateCollisionEnable() {

	if (isWaitToFirstAttack_) {
		if (!transform_.IsTransition()) {

			firstAttackCollisionEnable_ = true;
		}
	} else {
		firstAttackCollisionEnable_ = false;
	}

	if (isWaitToSecondAttackEnable_) {
		if (!transform_.IsTransition()) {

			secondAttackCollisionEnable_ = true;
		}
	} else {
		secondAttackCollisionEnable_ = false;
	}

	if (isWaitToThirdAttackEnable_) {
		if (!transform_.IsTransition()) {

			thirdAttackCollisionEnable_ = true;
		}
	} else {
		thirdAttackCollisionEnable_ = false;
	}
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