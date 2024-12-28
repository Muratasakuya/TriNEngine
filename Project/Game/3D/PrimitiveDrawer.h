#pragma once

//============================================================================*/
//	include
//============================================================================*/
#include <Engine/CBuffer/VertexBuffer.h>
#include <Engine/CBuffer/CameraBuffer.h>
#include <Engine/CBuffer/MaterialBuffer.h>
#include <Game/3D/Collision/CollisionShape.h>
#include <Game/Utility/Direction.h>
#include <Lib/MathUtils/Vector2.h>

// c++
#include <cstdint>
#include <numbers>
#include <unordered_map>

//============================================================================*/
//	LineColor
//============================================================================*/

enum class LineColor {

	White, // 白
	Black, // 黒
	Red,   // 赤
	Green, // 緑
	Blue,  // 青
};

// front
class PipelineManager;

//============================================================================*/
//	PrimitiveDrawer class
//============================================================================*/
class PrimitiveDrawer {
private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	// 線分の最大数
	static const uint32_t kMaxLineCount_ = 2048;
	// 線分の頂点数
	static const uint32_t kVertexCountLine_ = 2;

public:
	//========================================================================*/
	//	public Methods
	//========================================================================*/

	static PrimitiveDrawer* GetInstance();
	static void Finalize();

	void Init(PipelineManager* pipeline);

	void Update();

	void DrawLine3D(const Vector3& pointA, const Vector3& pointB, const LineColor& color = LineColor::White);

	void DrawGrid();

	void DrawSphere(float radius, const Vector3& centerPos, const LineColor& color);

	void DrawOBB(const Vector3& size, const Quaternion& rotate, const Vector3& centerPos, const LineColor& color);

	void DrawAABB(const AABB& aabb, const LineColor& color);

	void Reset();

private:
	//========================================================================*/
	//	private Methods
	//========================================================================*/

	//========================================================================*/
	//* variables

	static PrimitiveDrawer* instance_;

	PipelineManager* pipeline_ = nullptr;

	PrimitiveVertexBuffer vertexBuffer_;

	std::unordered_map<LineColor, PrimitiveMaterial> lineMaterials_;

	// 線分使用インデックス
	uint32_t indexLine_;

	//========================================================================*/
	//* functions

	PrimitiveDrawer() = default;
	~PrimitiveDrawer() = default;

	PrimitiveDrawer(const PrimitiveDrawer&) = delete;
	PrimitiveDrawer& operator=(const PrimitiveDrawer&) = delete;
	PrimitiveDrawer(PrimitiveDrawer&&) = delete;
	PrimitiveDrawer& operator=(PrimitiveDrawer&&) = delete;

};