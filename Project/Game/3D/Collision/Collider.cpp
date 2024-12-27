#include "Collider.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/PrimitiveDrawer.h>

//============================================================================*/
//	Collider classMethods
//============================================================================*/

void Collider::SetCollisionShapeSphere() {

	shape_ = CollisionShapes::Sphere::Default();
	shapeType_ = ShapeType::Type_Sphere;
}

void Collider::SetCollisionShapeOBB() {

	shape_ = CollisionShapes::OBB::Default();;
	shapeType_ = ShapeType::Type_OBB;
}

void Collider::OBBUpdate() {

	if (shape_ && std::holds_alternative <CollisionShapes::OBB>(*shape_)) {
		CollisionShapes::OBB& obb = std::get<CollisionShapes::OBB>(*shape_);

		obb.center = centerPos_;
		obb.rotate = rotate_;
		obb.size = size_;
	} else {
		assert(false && "collisionShape is not OBB");
	}
}

void Collider::DrawCollider() {

	std::visit([&](const auto& shape) {
		using ShapeType = std::decay_t<decltype(shape)>;

		//* Sphere *//
		if constexpr (std::is_same_v<ShapeType, CollisionShapes::Sphere>) {

			PrimitiveDrawer::GetInstance()->DrawSphere(shape.radius, centerPos_, LineColor::Red);
		}
		//* OBB *//
		else if constexpr (std::is_same_v<ShapeType, CollisionShapes::OBB>) {

			PrimitiveDrawer::GetInstance()->DrawOBB(size_, rotate_, centerPos_, LineColor::Red);
		}
		}, shape_.value());

}