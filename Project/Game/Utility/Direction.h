#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>

//============================================================================*/
//	Direction
//============================================================================*/

enum class ObjectDirection {


};

namespace Direction {

	// 右方向 (+X 軸)
	inline Vector3 Right() {
		return Vector3(1.0f, 0.0f, 0.0f);
	}

	// 左方向 (-X 軸)
	inline Vector3 Left() {
		return Vector3(-1.0f, 0.0f, 0.0f);
	}

	// 上方向 (+Y 軸)
	inline Vector3 Up() {
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	// 下方向 (-Y 軸)
	inline Vector3 Down() {
		return Vector3(0.0f, -1.0f, 0.0f);
	}

	// 前方向 (+Z 軸)
	inline Vector3 Forward() {
		return Vector3(0.0f, 0.0f, 1.0f);
	}

	// 後方向 (-Z 軸)
	inline Vector3 Backward() {
		return Vector3(0.0f, 0.0f, -1.0f);
	}

	// 斜め方向 (平面内: XY, XZ, YZ)
	inline Vector3 UpRight() {
		return Vector3(1.0f, 1.0f, 0.0f).Normalize();
	} // +X, +Y

	inline Vector3 UpLeft() {
		return Vector3(-1.0f, 1.0f, 0.0f).Normalize();
	} // -X, +Y

	inline Vector3 DownRight() {
		return Vector3(1.0f, -1.0f, 0.0f).Normalize();
	} // +X, -Y

	inline Vector3 DownLeft() {
		return Vector3(-1.0f, -1.0f, 0.0f).Normalize();
	} // -X, -Y

	// 斜め方向 (3次元空間: XYZ)
	inline Vector3 UpForwardRight() {
		return Vector3(1.0f, 1.0f, 1.0f).Normalize();
	}  // +X, +Y, +Z

	inline Vector3 UpForwardLeft() {
		return Vector3(-1.0f, 1.0f, 1.0f).Normalize();
	}  // -X, +Y, +Z

	inline Vector3 DownBackwardRight() {
		return Vector3(1.0f, -1.0f, -1.0f).Normalize();
	} // +X, -Y, -Z

	inline Vector3 DownBackwardLeft() {
		return Vector3(-1.0f, -1.0f, -1.0f).Normalize();
	} // -X, -Y, -Z
}