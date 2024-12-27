#include "Camera2D.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/Utility/Environment.h>

//============================================================================*/
//	Camera2D class
//============================================================================*/

void Camera2D::Init() {

	Matrix4x4 matrix = Matrix4x4::MakeAffineMatrix(
		Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
	Matrix4x4 viewMatrix = Matrix4x4::Inverse(matrix);

	Matrix4x4 orthoMatrix =
		Matrix4x4::MakeOrthographicMatrix(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 100.0f);

	viewOrthoMatrix_ = viewMatrix * orthoMatrix;

}