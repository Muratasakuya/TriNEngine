#include "BaseGameObject.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>
#include <Game/Utility/Direction.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	BaseGameObject classMethods
//============================================================================*/

void BaseGameObject::Init(const std::string& modelName) {

	model_ = std::make_unique<Model>();
	model_->Init(modelName);

	transform_.Init();

	uvTransform.scale = Vector3(1.0f, 1.0f, 1.0f);

	materials_.resize(model_->GetMeshNum());
	for (auto& material : materials_) {

		material.Init();
		material.properties.color = Color::White();
		material.properties.enableLighting = true;
		material.properties.enableHalfLambert = true;
	}

}

void BaseGameObject::Update() {

	transform_.Update();
	for (auto& material : materials_) {

		material.Update();
	}
}

void BaseGameObject::Draw(RendererPipelineType pipeline) {

	model_->Draw(transform_, materials_, pipeline);
}

void BaseGameObject::DrawShadowDepth() {

	model_->DrawShadowDepth(transform_);
}

void BaseGameObject::TransformImGui() {

	if (ImGui::TreeNode("Transform")) {

		if (ImGui::Button("Save")) {
			IBaseGameObject::SaveJsonForTransform(transform_);
		}

		ImGui::DragFloat3("Scale", &transform_.scale.x, 0.01f);
		ImGui::DragFloat3("Translate", &transform_.translation.x, 0.01f);
		if (ImGui::Button("Reset Rotation")) {
			transform_.rotation.Init();
		}

		ImGui::TreePop();
	}

	KeyRotate();
}

void BaseGameObject::KeyRotate() {

	// ないよりはまし

	const float rotationSpeed = 2.0f * std::numbers::pi_v<float> / 180.0f;

	if (Input::GetInstance()->PushKey(DIK_UP)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Right(), rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	} else if (Input::GetInstance()->PushKey(DIK_DOWN)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Right(), -rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Up(), rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	} else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Up(), -rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	}
	if (Input::GetInstance()->PushKey(DIK_Q)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Forward(), rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	} else if (Input::GetInstance()->PushKey(DIK_E)) {

		Quaternion rotation = Quaternion::MakeRotateAxisAngleQuaternion(Direction::Forward(), -rotationSpeed);
		transform_.rotation = transform_.rotation * rotation;
	}

}

void BaseGameObject::SetWorldTransform(const WorldTransform& transform) {

	transform_.translation = transform.translation;
	transform_.scale = transform.scale;
	transform_.rotation = transform.rotation;
}

void BaseGameObject::SetParent(const WorldTransform& transform) {

	transform_.parent = &transform;
}

void BaseGameObject::SetTranslate(const Vector3& translate) {

	transform_.translation = translate;
}

void BaseGameObject::SetRotate(const Quaternion& rotate) {

	transform_.rotation = rotate;
}