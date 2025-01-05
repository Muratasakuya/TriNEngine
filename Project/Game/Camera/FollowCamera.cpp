#include "FollowCamera.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/Utility/Direction.h>
#include <Game/System/GameSystem.h>
#include <Engine/Process/Input.h>
#include <Lib/Adapter/JsonAdapter.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	FollowCamera classMethods
//============================================================================*/

void FollowCamera::Init(Matrix4x4 projectionMatrix) {

	transform_.Init();

	ApplyJson();

	transform_.rotation = Quaternion::EulerToQuaternion(eulerRotate_);

	projectionMatrix_ = projectionMatrix;

}

void FollowCamera::Update() {

	if (!target_) {
		return;
	}

	// カメラ移動、回転
	Move();

	transform_.Update();

	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform_.rotation);
	Matrix4x4 worldMatrix = Matrix4x4::MakeIdentity4x4();
	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(transform_.translation);
	worldMatrix = rotateMatrix * translateMatrix;

	Matrix4x4 viewMatrix = Matrix4x4::Inverse(worldMatrix);
	viewProjectionMatrix_ = viewMatrix * projectionMatrix_;

}

void FollowCamera::Move() {

	Vector3 rotate{};
	rotate.Init();

	rotate.y = Input::GetInstance()->GetRightStickVal().x; // Y軸回転
	rotate.x = Input::GetInstance()->GetRightStickVal().y; // X軸回転
	eulerRotate_.y += rotate.Normalize().y / rotateLerpRate_;
	eulerRotate_.x += rotate.Normalize().x / 96.0f;
	if (eulerRotate_.x <= -0.1f) {
		eulerRotate_.x = -0.1f;
	}
	if (eulerRotate_.x >= 0.55f) {
		eulerRotate_.x = 0.55f;
	}

	transform_.rotation = Quaternion::EulerToQuaternion(eulerRotate_);
	interTarget_ = Vector3::Lerp(interTarget_, target_->translation, interLerpRate_);

	Vector3 offset{};
	offset.Init();

	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(transform_.rotation);
	offset = Vector3::TransferNormal(offset_, rotateMatrix);

	// オフセット分座標をずらす
	transform_.translation = interTarget_ + offset;

}

void FollowCamera::ImGui() {

	if (ImGui::Button("Save")) {
		SaveJson();
	}

	ImGui::DragFloat("interLerpRate", &interLerpRate_, 0.01f);
	ImGui::DragFloat("rotateLerpRate", &rotateLerpRate_, 0.01f);
	ImGui::DragFloat("rotateX", &eulerRotate_.x, 0.01f);
	ImGui::DragFloat3("offset", &offset_.x, 0.01f);

}

void FollowCamera::Reset() {

	target_ = nullptr;
}

void FollowCamera::SetTarget(const AnimationTransform* target) {

	target_ = target;
	interTarget_ = target->translation;
	Update();
}

void FollowCamera::ApplyJson() {

	Json data = JsonAdapter::Load(parentFolderName_ + "FollowCameraParameter.json");

	eulerRotate_.x = data["rotateX"];
	interLerpRate_ = data["interLerpRate"];
	rotateLerpRate_ = data["rotateLerpRate"];
	offset_ = JsonAdapter::ToVector3(data["offsetTranslate"]);

}

void FollowCamera::SaveJson() {

	Json data;

	data["rotateX"] = eulerRotate_.x;
	data["interLerpRate"] = interLerpRate_;
	data["rotateLerpRate"] = rotateLerpRate_;
	data["offsetTranslate"] = JsonAdapter::FromVector3(offset_);

	JsonAdapter::Save(parentFolderName_ + "FollowCameraParameter.json", data);

}