#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Game/3D/Collision/Collider.h>
#include <Game/Utility/Direction.h>
#include <Lib/MathUtils/Matrix4x4.h>

// c++
#include <list>
#include <array>
#include <utility>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <deque>

//============================================================================*/
//	CollisionManager class
//============================================================================*/
class CollisionManager {
public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	CollisionManager() = default;
	~CollisionManager() = default;

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

	void ClearAllColliders();

	void UpdateAllCollisions();

	void DisplayCollisionLogs();

	void ResetLog();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* hashOperator

		//* XOR *//
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	//========================================================================*/
	//* variables

	std::list<Collider*> colliders_;
	std::unordered_set<std::pair<Collider*, Collider*>, pair_hash> preCollisions_;

	//* log

	struct CollisionLogEntry {

		std::string message;
		std::chrono::time_point<std::chrono::steady_clock> enterTime;
		std::optional<std::chrono::time_point<std::chrono::steady_clock>> exitTime = std::nullopt;
	};
	std::deque<CollisionLogEntry> collisionLogs_;
	static constexpr size_t kMaxLogSize = 7;

	//========================================================================*/
	//* functions

	bool IsColliding(Collider* colliderA, Collider* colliderB);

	bool SphereToSphere(const CollisionShapes::Sphere& sphereA, const CollisionShapes::Sphere& sphereB,
		const Vector3& centerA, const Vector3& centerB);

	bool SphereToOBB(const CollisionShapes::Sphere& sphere, const CollisionShapes::OBB& obb,
		const Vector3& sphereCenter);

	bool OBBToOBB(const CollisionShapes::OBB& obbA, const CollisionShapes::OBB& obbB);

	void UpdateEnterLog(Collider* colliderA, Collider* colliderB);

	void UpdateStayLog(Collider* colliderA, Collider* colliderB);

	void UpdateExitLog(Collider* colliderA, Collider* colliderB);

};