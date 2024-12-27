#include "Quaternion.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Matrix4x4.h>
#include <Engine/Utility/AssetStructure.h>

//============================================================================*/
//	Quaternion classMethods
//============================================================================*/

Quaternion Quaternion::operator+(const Quaternion& other) const {
	return { x + other.x, y + other.y, z + other.z, w + other.w };
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	return { w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w,
			w * other.w - x * other.x - y * other.y - z * other.z };
}

Quaternion Quaternion::operator-() const {
	return { -x, -y, -z, -w };
}

Quaternion operator*(float scalar, const Quaternion& q) {
	return { scalar * q.x,scalar * q.y ,scalar * q.z ,scalar * q.w };
}

Quaternion Quaternion::operator*(float scalar) const {
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Vector3 Quaternion::operator*(const Vector3& v) const {

	Quaternion qv(0, v.x, v.y, v.z);
	Quaternion qConjugate = this->Inverse(Quaternion(*this));

	Quaternion result = (*this) * qv * qConjugate;
	return Vector3(result.x, result.y, result.z);
}

void Quaternion::Init() {

	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 1.0f;
}

Quaternion Quaternion::EulerToQuaternion(const Vector3& euler) {

	// オイラー角（ピッチ、ヨー、ロール）をラジアンに変換
	float pitch = euler.x * 0.5f;
	float yaw = euler.y * 0.5f;
	float roll = euler.z * 0.5f;

	// 各角度に対するサインとコサインを計算
	float sinPitch = std::sin(pitch);
	float cosPitch = std::cos(pitch);
	float sinYaw = std::sin(yaw);
	float cosYaw = std::cos(yaw);
	float sinRoll = std::sin(roll);
	float cosRoll = std::cos(roll);

	// クォータニオンの各成分を計算
	Quaternion q;
	q.w = cosYaw * cosPitch * cosRoll + sinYaw * sinPitch * sinRoll;
	q.x = cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll;
	q.y = sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll;
	q.z = cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll;

	return q;
}

Vector3 Quaternion::ToEulerAngles(const Quaternion& quaternion) {

	Vector3 angles;

	// ロール（X軸回転）
	float sinr_cosp = 2.0f * (quaternion.w * quaternion.x + quaternion.y * quaternion.z);
	float cosr_cosp = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.y * quaternion.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// ピッチ（Y軸回転）
	float sinp = 2.0f * (quaternion.w * quaternion.y - quaternion.z * quaternion.x);
	if (std::abs(sinp) >= 1.0f)
		angles.y = std::copysign(std::numbers::pi_v<float> / 2.0f, sinp);
	else
		angles.y = std::asin(sinp);

	// ヨー（Z軸回転）
	float siny_cosp = 2.0f * (quaternion.w * quaternion.z + quaternion.x * quaternion.y);
	float cosy_cosp = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}

Quaternion Quaternion::Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion result;

	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;

	return result;
}

Quaternion Quaternion::IdentityQuaternion() {

	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Quaternion::Conjugate(const Quaternion& quaternion) {

	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

float Quaternion::Norm(const Quaternion& quaternion) {

	return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y +
		quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Quaternion::Normalize(const Quaternion& quaternion) {

	float norm = Norm(quaternion);
	return { quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm };
}

Quaternion Quaternion::Inverse(const Quaternion& quaternion) {

	Quaternion conjugate = Conjugate(quaternion);
	float normSq = Norm(quaternion) * Norm(quaternion);

	return { conjugate.x / normSq, conjugate.y / normSq, conjugate.z / normSq, conjugate.w / normSq };
}

Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {

	Quaternion result{};

	float halfAngle = angle * 0.5f;
	float sinHalfAngle = std::sin(halfAngle);
	result.x = axis.x * sinHalfAngle;
	result.y = axis.y * sinHalfAngle;
	result.z = axis.z * sinHalfAngle;
	result.w = std::cos(halfAngle);

	return result;
}

Vector3 Quaternion::RotateVector(const Vector3& vector, const Quaternion& quaternion) {

	Quaternion qVector{ vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConjugate = Conjugate(quaternion);
	Quaternion qResult = Multiply(Multiply(quaternion, qVector), qConjugate);

	return { qResult.x, qResult.y, qResult.z };
}

Matrix4x4 Quaternion::MakeRotateMatrix(const Quaternion& quaternion) {

	Matrix4x4 result;
	float xx = quaternion.x * quaternion.x;
	float yy = quaternion.y * quaternion.y;
	float zz = quaternion.z * quaternion.z;
	float ww = quaternion.w * quaternion.w;
	float xy = quaternion.x * quaternion.y;
	float xz = quaternion.x * quaternion.z;
	float yz = quaternion.y * quaternion.z;
	float wx = quaternion.w * quaternion.x;
	float wy = quaternion.w * quaternion.y;
	float wz = quaternion.w * quaternion.z;

	result.m[0][0] = ww + xx - yy - zz;
	result.m[0][1] = 2.0f * (xy + wz);
	result.m[0][2] = 2.0f * (xz - wy);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 2.0f * (xy - wz);
	result.m[1][1] = ww - xx + yy - zz;
	result.m[1][2] = 2.0f * (yz + wx);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 2.0f * (xz + wy);
	result.m[2][1] = 2.0f * (yz - wx);
	result.m[2][2] = ww - xx - yy + zz;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Quaternion Quaternion::Slerp(Quaternion q0, const Quaternion& q1, float t) {

	/// q0とq1の内積
	float dot = Dot(q0, q1);

	// 内積が負の場合、もう片方の回転を利用する
	if (dot < 0.0f) {

		q0 = -q0;
		dot = -dot;
	}

	if (dot >= 1.0f - FLT_EPSILON) {

		return (1.0f - t) * q0 + t * q1;
	}

	// なす角を求める
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	// 補完係数を計算
	float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	float scale1 = std::sin(t * theta) / sinTheta;

	// 補完後のクォータニオンを求める
	return q0 * scale0 + q1 * scale1;
}

float Quaternion::Dot(const Quaternion& q0, const Quaternion& q1) {

	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

Quaternion Quaternion::CalculateValue(const std::vector<Keyframe<Quaternion>>& keyframes, float time) {

	// キーがないものは返す値が分からないのでアウト
	assert(!keyframes.empty());

	// キーが1つか、時刻がキーフレーム前なら最初の値とする
	if (keyframes.size() == 1 || time <= keyframes[0].time) {

		return keyframes[0].value;
	}

	for (size_t index = 0; index < keyframes.size(); ++index) {

		size_t nextIndex = index + 1;

		// indexとnextIndexの2つのkeyframeを取得して範囲内に時刻があるかを判定
		if (keyframes[index].time <= time && time <= keyframes[nextIndex].time) {

			// 範囲内を補完する
			float t =
				(time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);

			return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
		}
	}

	// ここまで来た場合は1番後の時刻よりも後ろなので最後の値を返す
	return (*keyframes.rbegin()).value;
}

Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up) {

	Vector3 f = forward.Normalize();
	Vector3 r = Vector3::Cross(up.Normalize(), f).Normalize();
	Vector3 u = Vector3::Cross(f, r);

	float m00 = r.x, m01 = u.x, m02 = f.x;
	float m10 = r.y, m11 = u.y, m12 = f.y;
	float m20 = r.z, m21 = u.z, m22 = f.z;

	float trace = m00 + m11 + m22;

	if (trace > 0.0f) {
		float s = std::sqrt(trace + 1.0f) * 0.5f;
		float invS = 0.25f / s;
		return { invS * (m21 - m12), invS * (m02 - m20), invS * (m10 - m01), s };
	}

	if (m00 > m11 && m00 > m22) {
		float s = std::sqrt(1.0f + m00 - m11 - m22) * 0.5f;
		float invS = 0.25f / s;
		return { s, invS * (m01 + m10), invS * (m02 + m20), invS * (m21 - m12) };
	}

	if (m11 > m22) {
		float s = std::sqrt(1.0f + m11 - m00 - m22) * 0.5f;
		float invS = 0.25f / s;
		return { invS * (m01 + m10), s, invS * (m12 + m21), invS * (m02 - m20) };
	}

	float s = std::sqrt(1.0f + m22 - m00 - m11) * 0.5f;
	float invS = 0.25f / s;
	return { invS * (m02 + m20), invS * (m12 + m21), s, invS * (m10 - m01) };
}

Quaternion Quaternion::LookAt(const Vector3& from, const Vector3& to, const Vector3& up) {

	Vector3 forward = (to - from).Normalize();
	Vector3 right = Vector3::Cross(up, forward).Normalize();
	Vector3 correctedUp = Vector3::Cross(forward, right);

	Matrix4x4 rotationMatrix = {
		right.x,      right.y,      right.z,      0.0f,
		correctedUp.x,correctedUp.y,correctedUp.z,0.0f,
		forward.x,    forward.y,    forward.z,    0.0f,
		0.0f,         0.0f,         0.0f,         1.0f
	};

	return Quaternion::FromRotationMatrix(rotationMatrix);
}

Quaternion Quaternion::FromRotationMatrix(const Matrix4x4& m) {

	Quaternion q;

	float trace = m.m[0][0] + m.m[1][1] + m.m[2][2];
	if (trace > 0.0f) {
		float s = sqrt(trace + 1.0f) * 2.0f; // 4 * q.w
		q.w = 0.25f * s;
		q.x = (m.m[2][1] - m.m[1][2]) / s;
		q.y = (m.m[0][2] - m.m[2][0]) / s;
		q.z = (m.m[1][0] - m.m[0][1]) / s;
	} else if ((m.m[0][0] > m.m[1][1]) && (m.m[0][0] > m.m[2][2])) {
		float s = sqrt(1.0f + m.m[0][0] - m.m[1][1] - m.m[2][2]) * 2.0f; // 4 * q.x
		q.w = (m.m[2][1] - m.m[1][2]) / s;
		q.x = 0.25f * s;
		q.y = (m.m[0][1] + m.m[1][0]) / s;
		q.z = (m.m[0][2] + m.m[2][0]) / s;
	} else if (m.m[1][1] > m.m[2][2]) {
		float s = sqrt(1.0f + m.m[1][1] - m.m[0][0] - m.m[2][2]) * 2.0f; // 4 * q.y
		q.w = (m.m[0][2] - m.m[2][0]) / s;
		q.x = (m.m[0][1] + m.m[1][0]) / s;
		q.y = 0.25f * s;
		q.z = (m.m[1][2] + m.m[2][1]) / s;
	} else {
		float s = sqrt(1.0f + m.m[2][2] - m.m[0][0] - m.m[1][1]) * 2.0f; // 4 * q.z
		q.w = (m.m[1][0] - m.m[0][1]) / s;
		q.x = (m.m[0][2] + m.m[2][0]) / s;
		q.y = (m.m[1][2] + m.m[2][1]) / s;
		q.z = 0.25f * s;
	}

	return q;
}