#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Matrix4x4.h>

//============================================================================*/
//	DebugCamera class
//============================================================================*/
class DebugCamera {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DebugCamera();
	~DebugCamera() = default;

	void Update(const Vector3& translate, const Vector3& rotation, const Matrix4x4& projectionMatrix);

	void ImGui();

	//* getter **//

	Vector3 GetRotate() const { return rotation_; }
	Vector3 GetTranslate() const { return translation_; }

	bool Enable() const { return enable_; }
	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }

	Matrix4x4 GetCameraMatrix() const { return matrix_; }

	//* setter *//

	void SetEnable(bool enable) { enable_ = enable; }

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Vector3 rotation_;
	Vector3 translation_;

	Matrix4x4 rotateMatrix_;         //* 回転行列
	Matrix4x4 matrix_;               //* World行列
	Matrix4x4 viewProjectionMatrix_; //* 渡す用の行列

	bool enable_; //* ON:OFF 切り替え

	//* parameter *//

	float zoomRate_; //* マウスホイール移動感度

	//========================================================================*/
	///* functions

	void Move(); //* 移動

};