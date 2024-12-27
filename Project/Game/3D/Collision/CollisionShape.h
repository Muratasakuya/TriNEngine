#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Lib/MathUtils/Vector3.h>
#include <Lib/MathUtils/Quaternion.h>

// c++
#include <variant>

//============================================================================*/
//	CollisionShape
//============================================================================*/

namespace CollisionShapes {

	struct Sphere {

		float radius;

		static Sphere Default() {
			Sphere sphere = {
				.radius = 1.0f
			};
			return sphere;
		};
	};

	struct OBB {

		Vector3 size;
		Vector3 center;
		Quaternion rotate;

		static OBB Default() {
			OBB obb = {
				.size = {1.0f,1.0f,1.0f},
				.center = {0.0f,0.0f,0.0f},
				.rotate = {0.0f,0.0f,0.0f}
			};
			return obb;
		};
	};

	using Shapes = std::variant<Sphere, OBB>;

};

enum class ShapeType {

	Type_Sphere,
	Type_OBB
};

struct AABB {

	Vector3 center; // AABBの中心
	Vector3 extent; // AABBの半径（各軸方向の幅/2）

	Vector3 GetMin() const { return center - extent; }
	Vector3 GetMax() const { return center + extent; }
};