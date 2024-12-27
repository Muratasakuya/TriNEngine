#pragma once

//============================================================================*/
//	include
//============================================================================*/

// c++
#include <vector>
#include <numbers>
#include <algorithm>

// front
class Vector3;
class Matrix4x4;
template <typename tValue>
struct Keyframe;

//============================================================================*/
//	Quaternion class
//============================================================================*/
class Quaternion {
public:

	float x, y, z, w;

	//========================================================================*/
	//	operators
	//========================================================================*/

	Quaternion operator+(const Quaternion& other) const;
	Quaternion operator*(const Quaternion& other) const;
	Quaternion operator-() const;

	Quaternion operator*(float scalar) const;
	friend Quaternion operator*(float scalar, const Quaternion& q);

	Vector3 operator*(const Vector3& v) const;

	//========================================================================*/
	//	functions
	//========================================================================*/

	void Init();

	static Quaternion EulerToQuaternion(const Vector3& euler);

	static Vector3 ToEulerAngles(const Quaternion& quaternion);

	static Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);

	static Quaternion IdentityQuaternion();

	static Quaternion Conjugate(const Quaternion& quaternion);

	static float Norm(const Quaternion& quaternion);

	static Quaternion Normalize(const Quaternion& quaternion);

	static Quaternion Inverse(const Quaternion& quaternion);

	static Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle);

	static Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion);

	static Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion);

	static Quaternion Slerp(Quaternion q0, const Quaternion& q1, float t);

	static float Dot(const Quaternion& q0, const Quaternion& q1);

	static Quaternion CalculateValue(const std::vector<Keyframe<Quaternion>>& keyframes, float time);

	static Quaternion LookRotation(const Vector3& forward, const Vector3& up);

	static Quaternion LookAt(const Vector3& from, const Vector3& to, const Vector3& up);

	static Quaternion FromRotationMatrix(const Matrix4x4& m);

};