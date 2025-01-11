#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/Transform.h>

//============================================================================*/
//	FollowCamera class
//============================================================================*/
class FollowCamera {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	FollowCamera() = default;
	~FollowCamera() = default;

	void Init(Matrix4x4 projectionMatrix);

	void Update();

	void ImGui();

	void Reset();

	void UpdateScreenShake();

	//* getter *//

	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	Matrix4x4 GetCameraMatrix() const { return matrix_; }

	Vector3 GetTranslate() const { return transform_.translation; }

	Vector3 GetRotate() const { return eulerRotate_; }

	//* setter *//

	void SetTarget(const AnimationTransform* target);

	void SetScreenShake() { isScreenShake_ = true; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	const std::string parentFolderName_ = "Camera/";

	const AnimationTransform* target_;

	WorldTransform transform_;

	Vector3 eulerRotate_;
	Vector3 interTarget_;

	Matrix4x4 matrix_;

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

	//* screenShake *//

	bool isScreenShake_;					//* 画面シェイク中かどうか
	float screenShakeIntensity_ = 0.25f;	//* 画面シェイクの強度
	float screenShakeDuration_ = 0.2f;		//* 画面シェイクの持続時間
	float screenShakeTimer_ = 0.0f;			//* シェイクの経過時間

	//* parameter *//

	Vector3 offset_;

	float rotateLerpRate_;
	float interLerpRate_;

	//========================================================================*/
	///* functions

	void Move();

	// json
	void ApplyJson();
	void SaveJson();

};