#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/CameraBuffer.h>

//============================================================================*/
//	Camera3D class
//============================================================================*/
class Camera3D {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	Camera3D() = default;
	~Camera3D() = default;

	void Init();

	void Update();

	void ImGui();

	//* getter *//

	Vector3 GetRotate() const { return rotation_; }
	Vector3 GetTranslate() const { return translation_; }

	Matrix4x4 GetCameraMatrix() const { return matrix_; }
	Matrix4x4 GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
	Matrix4x4 GetProjectionMatrix() const { return projectionMatrix_; }

	CameraBuffer GetCameraBuffer() const { return cameraBuffer_; }
	ViewProjectionBuffer GetViewProBuffer() const { return viewProBuffer_; }

	//* setter *//

	void SetCamera(const Matrix4x4& viewProMatrix, const Vector3& translate);

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	Vector3 rotation_;
	Vector3 translation_;

	Matrix4x4 matrix_;

	Matrix4x4 viewMatrix_;
	Matrix4x4 projectionMatrix_;

	Matrix4x4 viewProjectionMatrix_;
	Matrix4x4 projectionInverseMatrix_;

	//* buffer *//

	CameraBuffer cameraBuffer_;
	ViewProjectionBuffer viewProBuffer_;

};