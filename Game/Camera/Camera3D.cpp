#include "Camera3D.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	Camera3D classMethods
//============================================================================*/

void Camera3D::Init() {

	rotation_ = { 0.26f,0.0f,0.0f };
	translation_ = Vector3(0.0f, 4.2f, -10.0f);

	matrix_ =
		Matrix4x4::MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), rotation_, translation_);
	viewMatrix_ = Matrix4x4::Inverse(matrix_);

	// アスペクト比
	float aspectRatio = static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight);
	projectionMatrix_ =
		Matrix4x4::MakePerspectiveFovMatrix(0.45f, aspectRatio, 0.1f, 100.0f);
	projectionInverseMatrix_ = Matrix4x4::Inverse(projectionMatrix_);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	cameraBuffer_.Init();
	viewProBuffer_.Init();

}

void Camera3D::Update() {

	matrix_ =
		Matrix4x4::MakeAffineMatrix(Vector3(1.0f, 1.0f, 1.0f), rotation_, translation_);
	viewMatrix_ = Matrix4x4::Inverse(matrix_);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	cameraBuffer_.Update(translation_);
	viewProBuffer_.Update(viewProjectionMatrix_);

}

void Camera3D::ImGui() {

	ImGui::DragFloat3("Rotate", &rotation_.x, 0.01f);
	ImGui::DragFloat3("Translate", &translation_.x, 0.01f);

}

void Camera3D::SetCamera(const Matrix4x4& viewProMatrix, const Vector3& translate) {

	viewProjectionMatrix_ = viewProMatrix;

	translation_ = translate;

	cameraBuffer_.Update(translate);
	viewProBuffer_.Update(viewProjectionMatrix_);

}