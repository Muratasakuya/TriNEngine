#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Engine/CBuffer/CameraBuffer.h>

//============================================================================*/
//	DemoDebugCamera class
//============================================================================*/
class DemoDebugCamera {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	DemoDebugCamera();
	~DemoDebugCamera() = default;

	void Update(const Matrix4x4& projectionMatrix);

	//* getter **//

	Vector3 GetRotate() const { return rotation_; }
	Vector3 GetTranslate() const { return translation_; }

	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	Matrix4x4 GetCameraMatrix() const { return matrix_; }

	ViewProjectionBuffer GetViewProBuffer() const { return viewProBuffer_; }

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

	ViewProjectionBuffer viewProBuffer_;

	//* parameter *//

	float zoomRate_; //* マウスホイール移動感度

	//========================================================================*/
	///* functions

	void Move(); //* 移動

};