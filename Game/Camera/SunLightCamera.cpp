#include "SunLightCamera.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>
#include <Game/3D/PrimitiveDrawer.h>

// directX
#include <DirectXMath.h>

// imgui
#include <imgui.h>

//============================================================================*/
//	SunLightCamera classMethods
//============================================================================*/

void SunLightCamera::Init() {

	translation_ = Vector3(0.0f, 14.0f, 0.0f);

	target_ = Vector3(0.0f, 0.0f, 0.0f);

	upDirection_ = Vector3(0.0f, 0.0f, -1.0f);

	orthoSize_ = 32.0f; // ジャギィが目立つがここはShaderで何とかする方が良さそう

	nearPlane_ = 1.0f;
	farPlane_ = 640.0f;

	Update();

	buffer_.Init();
	buffer_.Update(viewProjectionMatrix_);

}

void SunLightCamera::Update() {

	DirectX::XMMATRIX dxProjectionMatrix = DirectX::XMMatrixOrthographicLH(
		orthoSize_,
		orthoSize_,
		nearPlane_,
		farPlane_
	);

	DirectX::XMFLOAT3 eye(translation_.x, translation_.y, translation_.z);
	DirectX::XMFLOAT3 target(target_.x, target_.y, target_.z);
	DirectX::XMFLOAT3 up(upDirection_.x, upDirection_.y, upDirection_.z);

	DirectX::XMMATRIX dxViewMatrix = DirectX::XMMatrixLookAtLH(
		DirectX::XMLoadFloat3(&eye),
		DirectX::XMLoadFloat3(&target),
		DirectX::XMLoadFloat3(&up));

	// 元のMatrix4x4に戻す
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&projectionMatrix_), dxProjectionMatrix);
	DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&viewMatrix_), dxViewMatrix);

	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix_;

	buffer_.Update(viewProjectionMatrix_);

}

void SunLightCamera::DrawDebug() {
}

void SunLightCamera::ImGui() {

	ImGui::Separator();
	ImGui::Text("sunLightCamera");

	ImGui::DragFloat("orthoSize", &orthoSize_, 1.0f);
	ImGui::DragFloat("nearPlane", &nearPlane_, 0.01f);
	ImGui::DragFloat("farPlane", &farPlane_);
	ImGui::DragFloat3("translation", &translation_.x, 0.01f, 0.0f, 1000.0f);
	ImGui::DragFloat3("target", &target_.x, 0.01f);
	ImGui::DragFloat3("upDirection", &upDirection_.x, 0.01f);

}

void SunLightCamera::SetTranslate(const Vector3& translate) {

	translation_.x = translate.x;
	translation_.z = translate.z;

	target_ = translate;

}