#include "DemoDebugCamera.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Process/Input.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	DemoDebugCamera class
//============================================================================*/

DemoDebugCamera::DemoDebugCamera() {

	// 最初は無効
	zoomRate_ = 1.0f;

	rotation_ = { 0.24f,0.54f,0.0f };
	translation_ = Vector3(-16.0f, 7.0f, -25.0f);

	viewProBuffer_.Init();

}

void DemoDebugCamera::Update(const Matrix4x4& projectionMatrix) {

	Move();

	rotateMatrix_ = Matrix4x4::MakeRotateMatrix(rotation_);
	matrix_ = Matrix4x4::MakeIdentity4x4();
	Vector3 offset = { 0.0f, 2.0f, -25.0f };
	offset = Vector3::TransferNormal(offset, rotateMatrix_);

	Matrix4x4 translateMatrix = Matrix4x4::MakeTranslateMatrix(translation_);
	Matrix4x4 scaleMatrix = Matrix4x4::MakeScaleMatrix({ 1.0f,1.0f,1.0f });
	matrix_ = Matrix4x4::Multiply(scaleMatrix, rotateMatrix_);
	matrix_ = Matrix4x4::Multiply(matrix_, translateMatrix);
	Matrix4x4  viewMatrix = Matrix4x4::Inverse(matrix_);

	viewProjectionMatrix_ = viewMatrix * projectionMatrix;

	viewProBuffer_.Update(viewProjectionMatrix_);

}

void DemoDebugCamera::Move() {

	float deltaX = Input::GetInstance()->GetMouseMoveValue().x;
	float deltaY = Input::GetInstance()->GetMouseMoveValue().y;

	const float rotateSpeed = 0.01f;
	const float panSpeed = 0.1f;

	// 右クリック + LeftCtrl
	if (Input::GetInstance()->PushMouseRight() && Input::GetInstance()->PushKey(DIK_LCONTROL)) {

		rotation_.x += deltaY * rotateSpeed;
		rotation_.y += deltaX * rotateSpeed;
	}

	// 中クリック + LeftCtrl
	if (Input::GetInstance()->PushMouseCenter() && Input::GetInstance()->PushKey(DIK_LCONTROL)) {

		Vector3 right = { panSpeed * deltaX, 0.0f, 0.0f };
		Vector3 up = { 0.0f, -panSpeed * deltaY, 0.0f };

		// 平行移動ベクトルを変換
		right = Vector3::TransferNormal(right, matrix_);
		up = Vector3::TransferNormal(up, matrix_);

		translation_ += right + up;
	}

	Vector3 forward = { 0.0f, 0.0f, Input::GetInstance()->GetMouseWheel() * zoomRate_ };
	forward = Vector3::TransferNormal(forward, rotateMatrix_);

	if (Input::GetInstance()->GetMouseWheel() != 0 && Input::GetInstance()->PushKey(DIK_LCONTROL)) {

		translation_ += forward;
	}
}