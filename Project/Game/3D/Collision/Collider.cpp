#include "Collider.h"

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/PrimitiveDrawer.h>
#include <Game/System/GameSystem.h>

//============================================================================*/
//	Collider classMethods
//============================================================================*/

Collider::~Collider() {
	GameSystem::GetCollision()->RemoveCollider(this);
}

void Collider::SetCollisionShapeSphere(const CollisionShapes::Sphere& sphere) {

	shape_ = sphere;
	shapeType_ = ShapeType::Type_Sphere;

	GameSystem::GetCollision()->AddCollider(this);
}

void Collider::SetCollisionShapeOBB(const CollisionShapes::OBB& obb) {

	shape_ = obb;
	shapeType_ = ShapeType::Type_OBB;

	GameSystem::GetCollision()->AddCollider(this);
}

void Collider::SphereUpdate() {

	if (shape_ && std::holds_alternative <CollisionShapes::Sphere>(*shape_)) {
		CollisionShapes::Sphere& sphere = std::get<CollisionShapes::Sphere>(*shape_);

		sphere.radius = radius_;

	} else {
		assert(false && "collisionShape is not Sphere");
	}
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