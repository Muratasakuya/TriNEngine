#include "CollisionManager.h"

//============================================================================*/
//	include
//============================================================================*/

// imgui
#include <imgui.h>

//============================================================================*/
//	CollisionManager classMethods
//============================================================================*/

void CollisionManager::AddCollider(Collider* collider) {

	colliders_.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider) {

	auto itA = std::find(colliders_.begin(), colliders_.end(), collider);
	if (itA != colliders_.end()) {
		colliders_.erase(itA);
	}

	for (auto itB = preCollisions_.begin(); itB != preCollisions_.end();) {
		if (itB->first == collider || itB->second == collider) {

			itB = preCollisions_.erase(itB);
		} else {
			++itB;
		}
	}
}

void CollisionManager::ClearAllColliders() {

	colliders_.clear();
	preCollisions_.clear();
}

void CollisionManager::UpdateAllCollisions() {

	if (colliders_.empty()) {
		return;
	}

	std::unordered_set<std::pair<Collider*, Collider*>, pair_hash> currentCollisions;

	for (auto itA = colliders_.begin(); itA != colliders_.end(); ++itA) {

		auto itB = itA;
		++itB;

		for (; itB != colliders_.end(); ++itB) {

			Collider* colliderA = *itA;
			Collider* colliderB = *itB;

			if (colliderA->GetTargetType() != colliderB->GetType() &&
				colliderB->GetTargetType() != colliderA->GetType()) {
				continue;
			}

			if (IsColliding(colliderA, colliderB)) {
				currentCollisions.insert({ colliderA, colliderB });

				if (preCollisions_.find({ colliderA, colliderB }) == preCollisions_.end()) {

					colliderA->OnCollisionEnter(colliderB);
					colliderB->OnCollisionEnter(colliderA);

					UpdateEnterLog(colliderA, colliderB);
				} else {

					colliderA->OnCollisionStay(colliderB);
					colliderB->OnCollisionStay(colliderA);

					UpdateStayLog(colliderA, colliderB);
				}
			}
		}
	}

	for (auto& collision : preCollisions_) {
		if (currentCollisions.find(collision) == currentCollisions.end()) {

			collision.first->OnCollisionExit(collision.second);
			collision.second->OnCollisionExit(collision.first);

			UpdateExitLog(collision.first, collision.second);
		}
	}

	preCollisions_ = currentCollisions;
}

void CollisionManager::DisplayCollisionLogs() {
#ifdef _DEBUG

	if (colliders_.empty()) {

		ImGui::Text("No CollisionSetting");
		ImGui::Separator();

		return;
	}

	ImGui::Text("CollisionLog");
	ImGui::Separator();
	if (ImGui::Button("Reset")) {
		collisionLogs_.clear();
	}

	for (const auto& log : collisionLogs_) {

		if (log.exitTime.has_value()) {

			auto durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(log.exitTime.value() - log.enterTime).count();
			auto durationS = std::chrono::duration_cast<std::chrono::seconds>(log.exitTime.value() - log.enterTime).count();
			ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "%s [%lld ms] [%lld s] ", log.message.c_str(), durationMS, durationS);
		} else {
			ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "%s", log.message.c_str());
		}
	}

#endif // _DEBUG
}

void CollisionManager::ResetLog() {

	collisionLogs_.clear();
}

void CollisionManager::UpdateEnterLog(Collider* colliderA, Collider* colliderB) {

	if (collisionLogs_.size() >= kMaxLogSize) {
		collisionLogs_.pop_front();
	}
	CollisionLogEntry logEntry;
	logEntry.message = "Enter: " + colliderA->GetName() + " and " + colliderB->GetName();
	logEntry.enterTime = std::chrono::steady_clock::now();
	collisionLogs_.emplace_back(logEntry);
}

void CollisionManager::UpdateStayLog(Collider* colliderA, Collider* colliderB) {

	for (const auto& log : collisionLogs_) {
		if (log.message.find("Stay~: " + colliderA->GetName() + " and " + colliderB->GetName()) != std::string::npos ||
			log.message.find("Stay~: " + colliderB->GetName() + " and " + colliderA->GetName()) != std::string::npos) {
			return;
		}
	}

	if (collisionLogs_.size() >= kMaxLogSize) {
		collisionLogs_.pop_front();
	}
	CollisionLogEntry logEntry;
	logEntry.message = "Stay~: " + colliderA->GetName() + " and " + colliderB->GetName();
	logEntry.enterTime = std::chrono::steady_clock::now();
	collisionLogs_.emplace_back(logEntry);
}

void CollisionManager::UpdateExitLog(Collider* colliderA, Collider* colliderB) {

	for (auto it = collisionLogs_.begin(); it != collisionLogs_.end(); ++it) {
		if (it->message.find("Stay~: " + colliderA->GetName() + " and " + colliderB->GetName()) != std::string::npos ||
			it->message.find("Stay~: " + colliderB->GetName() + " and " + colliderA->GetName()) != std::string::npos) {

			collisionLogs_.erase(it);
			break;
		}
	}

	for (auto& log : collisionLogs_) {
		if (log.message.find("Enter: " + colliderA->GetName() + " and " + colliderB->GetName()) != std::string::npos ||
			log.message.find("Enter: " + colliderB->GetName() + " and " + colliderA->GetName()) != std::string::npos) {
			if (!log.exitTime.has_value()) {

				log.exitTime = std::chrono::steady_clock::now();
				log.message = "Exit: " + colliderA->GetName() + " and " + colliderB->GetName();  // ログの内容を更新
				return;
			}
		}
	}
}

//============================================================================*/
//	Collision Methods
//============================================================================*/

bool CollisionManager::IsColliding(Collider* colliderA, Collider* colliderB) {

	//* Type && Center *//

	const auto& shapeA = colliderA->GetCollisionShape();
	const auto& shapeB = colliderB->GetCollisionShape();

	const Vector3 centerA = colliderA->GetCenterPos();
	const Vector3 centerB = colliderB->GetCenterPos();

	return std::visit([&](const auto& shapeA, const auto& shapeB) {

		using ShapeTypeA = std::decay_t<decltype(shapeA)>;
		using ShapeTypeB = std::decay_t<decltype(shapeB)>;

		if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::Sphere>) {

			return SphereToSphere(shapeA, shapeB, centerA, centerB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::Sphere> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return SphereToOBB(shapeA, shapeB, centerA);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::OBB> && std::is_same_v<ShapeTypeB, CollisionShapes::Sphere>) {

			return SphereToOBB(shapeB, shapeA, centerB);
		} else if constexpr (std::is_same_v<ShapeTypeA, CollisionShapes::OBB> && std::is_same_v<ShapeTypeB, CollisionShapes::OBB>) {

			return OBBToOBB(shapeA, shapeB);
		} else {

			return false;
		}
		}, shapeA, shapeB);
}

bool CollisionManager::SphereToSphere(
	const CollisionShapes::Sphere& sphereA, const CollisionShapes::Sphere& sphereB,
	const Vector3& centerA, const Vector3& centerB) {

	float distance =Vector3(centerA - centerB).Length();
	if (distance <= sphereA.radius + sphereB.radius) {
		return true;
	}

	return false;
}

bool CollisionManager::SphereToOBB(
	const CollisionShapes::Sphere& sphere, const CollisionShapes::OBB& obb,
	const Vector3& sphereCenter) {

	Matrix4x4 rotateMatrix = Quaternion::MakeRotateMatrix(obb.rotate);

	Vector3 orientations[3];
	orientations[0] = Vector3::Transform(Direction::Right(), rotateMatrix);
	orientations[1] = Vector3::Transform(Direction::Up(), rotateMatrix);
	orientations[2] = Vector3::Transform(Direction::Forward(), rotateMatrix);

	Vector3 localSphereCenter = sphereCenter - obb.center;
	Vector3 closestPoint = obb.center;

	for (int i = 0; i < 3; ++i) {

		float distance = Vector3::Dot(localSphereCenter, orientations[i]);
		float halfSize = (i == 0) ? obb.size.x : (i == 1) ? obb.size.y : obb.size.z;

		if (distance > halfSize) {
			distance = halfSize;
		} else if (distance < -halfSize) {
			distance = -halfSize;
		}

		closestPoint += distance * orientations[i];
	}

	Vector3 diff = closestPoint - sphereCenter;
	float distanceSquared = Vector3::Dot(diff, diff);

	return distanceSquared <= (sphere.radius * sphere.radius);
}

bool CollisionManager::OBBToOBB(const CollisionShapes::OBB& obbA, const CollisionShapes::OBB& obbB) {

	auto CalculateProjection =
		[](const CollisionShapes::OBB& obb, const Vector3& axis, const Vector3* axes) -> float {
		return std::abs(obb.size.x * Vector3::Dot(axes[0], axis)) +
			std::abs(obb.size.y * Vector3::Dot(axes[1], axis)) +
			std::abs(obb.size.z * Vector3::Dot(axes[2], axis));
		};

	auto GetOBBAxes = [](const CollisionShapes::OBB& obb) -> std::array<Vector3, 3> {
		Matrix4x4 rotationMatrix = Quaternion::MakeRotateMatrix(obb.rotate);
		return {
			Vector3::Transform(Direction::Right(), rotationMatrix),
			Vector3::Transform(Direction::Up(), rotationMatrix),
			Vector3::Transform(Direction::Forward(), rotationMatrix)
		};
		};

	auto obbAxesA = GetOBBAxes(obbA);
	auto obbAxesB = GetOBBAxes(obbB);

	std::vector<Vector3> axes;
	axes.insert(axes.end(), obbAxesA.begin(), obbAxesA.end());
	axes.insert(axes.end(), obbAxesB.begin(), obbAxesB.end());
	for (const auto& axisA : obbAxesA) {
		for (const auto& axisB : obbAxesB) {
			axes.push_back(Vector3::Cross(axisA, axisB));
		}
	}

	for (const auto& axis : axes) {
		if (axis.Length() < std::numeric_limits<float>::epsilon()) {
			continue;
		}

		Vector3 normalizedAxis = axis.Normalize();

		float obbAProjection = CalculateProjection(obbA, normalizedAxis, obbAxesA.data());
		float obbBProjection = CalculateProjection(obbB, normalizedAxis, obbAxesB.data());
		float distance = std::abs(Vector3::Dot(obbA.center - obbB.center, normalizedAxis));

		if (distance > obbAProjection + obbBProjection) {
			return false;
		}
	}

	return true;
}