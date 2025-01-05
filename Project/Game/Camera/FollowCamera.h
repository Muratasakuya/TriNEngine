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

	//* getter *//

	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	Vector3 GetTranslate() const { return transform_.translation; }

	Vector3 GetRotate() const { return eulerRotate_; }

	//* setter *//

	void SetTarget(const AnimationTransform* target);

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

	Matrix4x4 projectionMatrix_;
	Matrix4x4 viewProjectionMatrix_;

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