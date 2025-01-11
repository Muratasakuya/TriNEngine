#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>

// front
class Quaternion;

//============================================================================*/
//	Matrix4x4 class
//============================================================================*/
class Matrix4x4 final {
public:

	Matrix4x4() {
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = 0.0f;
			}
		}
	}
	Matrix4x4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) {
		m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
		m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
		m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
		m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
	};

	float m[4][4];

	//========================================================================*/
	//	operators
	//========================================================================*/

	Matrix4x4 operator+(const Matrix4x4& other) const;
	Matrix4x4 operator-(const Matrix4x4& other) const;
	Matrix4x4 operator*(const Matrix4x4& other) const;
	Matrix4x4 operator/(float scalar) const;
	
	Matrix4x4& operator+=(const Matrix4x4& other);
	Matrix4x4& operator-=(const Matrix4x4& other);
	Matrix4x4& operator*=(const Matrix4x4& other);
	Matrix4x4& operator/=(const Matrix4x4& other);

	//========================================================================*/
	//	functions
	//========================================================================*/

	static Matrix4x4 Init();

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

	static Matrix4x4 Inverse(const Matrix4x4& m);

	static Matrix4x4 Transpose(const Matrix4x4& m);

	static Matrix4x4 MakeIdentity4x4();

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);

	static Matrix4x4 MakePitchMatrix(float radian);

	static Matrix4x4 MakeYawMatrix(float radian);

	static Matrix4x4 MakeRollMatrix(float radian);

	static Matrix4x4 MakeRotateMatrix(const Vector3& rotate);

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	static Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);
	static Matrix4x4 MakeShadowOrthographicMatrix(float width, float height, float nearClip, float farClip);

	static Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

	static Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

	static Matrix4x4 MakeAxisAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate);

	static Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to);

};